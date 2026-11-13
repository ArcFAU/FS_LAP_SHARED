# NX Open journal, read-only: the assembly tree of a top part with every component's position and body count.
#   run_journal.exe dump_tree.py -args <assembly.prt> <out.json>
# Works on copies in D:\FSLap_CAD only; it never saves a part.
import json
import os
import sys
import time
import traceback

import NXOpen
import NXOpen.Assemblies

asm_path, out_path = sys.argv[1], sys.argv[2]
s = NXOpen.Session.GetSession()
t0 = time.time()
out = {'part': asm_path, 'errors': [], 'components': []}


def walk(comp, level, parent):
    try:
        origin, matrix = comp.GetPosition()
        proto = comp.Prototype
        path = proto.FullPath if isinstance(proto, NXOpen.Part) else ''
        bodies = 0
        if isinstance(proto, NXOpen.Part):
            bodies = sum(1 for b in proto.Bodies if b.IsSolidBody)
        m = matrix
        out['components'].append({'level': level, 'name': comp.DisplayName, 'refset': comp.ReferenceSet, 'parent': parent, 'file': path, 'suppressed': comp.IsSuppressed,
                                  'origin': [origin.X, origin.Y, origin.Z], 'xrow': [m.Xx, m.Xy, m.Xz], 'yrow': [m.Yx, m.Yy, m.Yz], 'zrow': [m.Zx, m.Zy, m.Zz],
                                  'solid_bodies': bodies, 'children': len(comp.GetChildren())})
    except Exception as ex:
        out['errors'].append(comp.DisplayName + ': ' + repr(ex))
    for child in comp.GetChildren():
        walk(child, level + 1, comp.DisplayName)


try:
    s.Parts.LoadOptions.ComponentsToLoad = NXOpen.LoadOptions.LoadComponents.All
    s.Parts.LoadOptions.UsePartialLoading = False
    s.Parts.LoadOptions.LoadLatest = False
    s.Parts.LoadOptions.AbortOnFailure = False   # a few small parts are missing from the model folder
    # D:\FSLap_CAD\flat holds every part once (make_flat.py): the saved paths point to another laptop, and the copy's subfolders hold 117 names twice
    s.Parts.LoadOptions.ComponentLoadMethod = NXOpen.LoadOptions.LoadMethod.SearchDirectories
    s.Parts.LoadOptions.SetSearchDirectories([r'D:\FSLap_CAD\flat'], [False])
    part, status = s.Parts.Open(asm_path)
    out['loaded_outside_copy'] = sorted({p.FullPath for p in s.Parts if not p.FullPath.lower().startswith('d:\\fslap_cad')})
    # NX reports the open as failed (670024) when a few components are missing, but the part is loaded: find it
    part = part or next((p for p in s.Parts if p.FullPath.lower() == os.path.abspath(asm_path).lower()), None)
    out['load_seconds'] = round(time.time() - t0, 1)
    try:
        out['load_failures'] = [status.GetPartName(i) + ': ' + str(status.GetStatus(i)) for i in range(status.NumberUnloadedParts)]
    except Exception as ex:
        out['errors'].append('load status: ' + repr(ex))
    root = part.ComponentAssembly.RootComponent
    out['top_bodies'] = sum(1 for b in part.Bodies if b.IsSolidBody)
    if root:
        for child in root.GetChildren():
            walk(child, 1, part.Leaf)
except Exception:
    out['errors'].append(traceback.format_exc())
out['seconds'] = round(time.time() - t0, 1)
with open(out_path, 'w', encoding='utf-8') as f:
    json.dump(out, f, indent=1)
print('TREE_DONE', len(out['components']), 'components', len(out.get('load_failures', [])), 'unloaded', len(out['errors']), 'errors', out['seconds'], 's')
