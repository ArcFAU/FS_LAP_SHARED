# NX Open journal, read-only: each job's solid bodies to one binary STL in the part's own coordinates.
#   run_journal.exe export_stl.py -args <jobs.json> <out folder>
# Opens copies from D:\FSLap_CAD only and closes them without saving. Skips STLs that already exist, so it can resume.
import json
import os
import re
import sys
import time

import NXOpen
import NXOpen.UF

def real_sheet(body):
    """A sheet that is part of the car: several faces, or one free-form face (the suspension cover).
    One plane, cylinder, cone, sphere or revolved face is a construction aid (planes 1 m wide, a 2 m helper cylinder)."""
    faces = body.GetFaces()
    if len(faces) != 1:
        return len(faces) > 1
    return int(str(faces[0].SolidFaceType).split('.')[-1]) >= 6   # 1 planar ... 5 revolution; 6 parametric and up are free-form



def signature(body):
    """face count and size in mm (sorted, so a mirror matches): enough to recognise the same body in a mirrored part"""
    pts = [(v.X, v.Y, v.Z) for e in body.GetEdges() for v in e.GetVertices()]
    size = sorted(round(max(p[i] for p in pts) - min(p[i] for p in pts)) for i in range(3)) if pts else []
    return (len(body.GetFaces()), tuple(size))


dropped = {}   # part name -> signatures of the linked copies left out of it

jobs_path, out_dir = sys.argv[1], sys.argv[2]
s = NXOpen.Session.GetSession()
uf = NXOpen.UF.UFSession.GetUFSession()
jobs = json.load(open(jobs_path))['jobs']
os.makedirs(out_dir, exist_ok=True)
report, t0 = {}, time.time()
ORDER = 'scpae'   # suspension and steering first, then chassis, powertrain, aero, electronics
for name, path in sorted(jobs.items(), key=lambda kv: (ORDER.find(kv[0][0]) % 6, kv[0])):
    target = os.path.join(out_dir, name + '.stl')
    if os.path.exists(target):
        continue
    assert path.lower().startswith('d:\\fslap_cad\\'), path   # never open anything outside the copy
    part = None
    try:
        part, status = s.Parts.Open(path)
        part = part or next((p for p in s.Parts if p.FullPath.lower() == path.lower()), None)
        solids = [b for b in part.Bodies if b.IsSolidBody or (b.IsSheetBody and real_sheet(b))] if part else []   # some covers are surfaces, not solids
        # only what the assembly shows: the MODEL reference set (it leaves out sheet-metal flat patterns, construction
        # and envelope bodies); a part without one gets its bodies that are not hidden
        model = next((r for r in part.GetAllReferenceSets() if r.Name == 'MODEL'), None) if part else None
        if model:
            members = {m.Tag for m in model.AskAllDirectMembers()}
            bodies = [b for b in solids if b.Tag in members]
        else:
            bodies = [b for b in solids if not b.IsBlanked]
        # a body that is only a linked copy of another part's body (WAVE link) duplicates that part: the louver carries the
        # endplate, the main element bracket the main element, the tubing bracket the undertray sidewall. Kept if it is all the part has.
        links = [b for b in bodies if {f.FeatureType for f in b.GetFeatures()} == {'LINKED_BODY'}]
        # a _MIR part mirrors the whole original, linked copy included: drop the body shaped like the original's dropped copy
        original = re.sub(r'_mir(_\d+)?$', '', name)
        if original != name:
            links += [b for b in bodies if b not in links and signature(b) in dropped.get(original, [])]
        if links and len(links) < len(bodies):
            dropped[name] = [signature(b) for b in links]
            bodies = [b for b in bodies if b not in links]
        if bodies:
            s.Parts.SetDisplay(part, False, True)
            # STL export writes WCS coordinates, and many parts were saved with a moved WCS: put it back on the absolute
            # system (in memory only; the part is closed without saving)
            wcs = part.WCS.CoordinateSystem
            moved = [round(v, 3) for v in (wcs.Origin.X, wcs.Origin.Y, wcs.Origin.Z)] != [0, 0, 0] or wcs.Orientation.Element.Xx < 0.999999 or wcs.Orientation.Element.Yy < 0.999999
            identity = NXOpen.Matrix3x3()
            identity.Xx, identity.Xy, identity.Xz, identity.Yx, identity.Yy, identity.Yz, identity.Zx, identity.Zy, identity.Zz = 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0
            part.WCS.SetOriginAndMatrix(NXOpen.Point3d(0.0, 0.0, 0.0), identity)
            stl = s.DexManager.CreateStlCreator()
            stl.AutoNormalGen = True
            stl.ChordalTol = 0.5   # mm
            stl.AdjacencyTol = 0.08
            stl.OutputFile = target + '.part.stl'
            stl.ExportSelectionBlock.Add(bodies)
            stl.Commit()
            stl.Destroy()
            os.replace(target + '.part.stl', target)
            report[name] = {'bodies': len(bodies), 'units': str(part.PartUnits), 'wcs_was_moved': moved, 'left_out': len(solids) - len(bodies), 'refset': 'MODEL' if model else 'visible', 'sheets': sum(1 for b in bodies if b.IsSheetBody), 'links_dropped': len(links) if len(links) < len(solids) else 0, 'only_links': bool(links) and len(links) == len(solids)}
        else:
            report[name] = {'bodies': 0, 'left_out': len(solids)}
    except Exception as ex:
        report[name] = {'error': repr(ex)}
    finally:
        if part:
            try:
                part.Close(NXOpen.BasePart.CloseWholeTree.TrueValue, NXOpen.BasePart.CloseModified.CloseModified, None)   # discards, never saves
            except Exception as ex:
                report.setdefault(name, {})['close'] = repr(ex)
rp = os.path.join(out_dir, 'report_%d.json' % int(t0))
json.dump(report, open(rp, 'w'), indent=1)
print('EXPORT_DONE', len(report), 'parts', sum(1 for r in report.values() if r.get('bodies')), 'with bodies', sum(1 for r in report.values() if 'error' in r), 'errors', round(time.time() - t0), 's')
