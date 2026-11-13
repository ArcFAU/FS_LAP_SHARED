# NX Open journal, read-only: dump a part's expressions, points, lines, datum CSYS and features to JSON.
#   run_journal.exe dump_part.py -args <part.prt> <out.json>
# Works on copies in D:\FSLap_CAD only; it never saves a part.
import json
import sys
import traceback

import NXOpen

part_path, out_path = sys.argv[1], sys.argv[2]
s = NXOpen.Session.GetSession()
out = {'part': part_path, 'errors': []}
try:
    part, status = s.Parts.Open(part_path)
    out['units'] = str(part.PartUnits)
    out['expressions'] = []
    for e in part.Expressions:
        try:
            out['expressions'].append({'name': e.Name, 'rhs': e.RightHandSide, 'value': e.Value if e.Type == 'Number' else None,
                                       'units': e.Units.Name if e.Units else '', 'type': e.Type})
        except Exception as ex:
            out['errors'].append('expression: ' + repr(ex))
    out['points'] = []
    for p in part.Points:
        c = p.Coordinates
        out['points'].append({'name': p.Name, 'journal': p.JournalIdentifier, 'xyz': [c.X, c.Y, c.Z], 'layer': p.Layer})
    out['lines'] = []
    for l in part.Lines:
        a, b = l.StartPoint, l.EndPoint
        out['lines'].append({'name': l.Name, 'journal': l.JournalIdentifier, 'a': [a.X, a.Y, a.Z], 'b': [b.X, b.Y, b.Z], 'layer': l.Layer})
    out['csys'] = []
    for c in part.CoordinateSystems:
        o, m = c.Origin, c.Orientation.Element
        out['csys'].append({'name': c.Name, 'journal': c.JournalIdentifier, 'origin': [o.X, o.Y, o.Z],
                            'x': [m.Xx, m.Xy, m.Xz], 'y': [m.Yx, m.Yy, m.Yz]})
    out['features'] = []
    for f in part.Features:
        try:
            out['features'].append({'name': f.GetFeatureName(), 'type': f.FeatureType, 'custom': f.Name, 'suppressed': f.Suppressed})
        except Exception as ex:
            out['errors'].append('feature: ' + repr(ex))
    out['sketches'] = []
    for sk in part.Sketches:
        geo = []
        for g in sk.GetAllGeometry():
            if isinstance(g, NXOpen.Line):
                a, b = g.StartPoint, g.EndPoint
                geo.append({'kind': 'line', 'name': g.Name, 'a': [a.X, a.Y, a.Z], 'b': [b.X, b.Y, b.Z]})
            elif isinstance(g, NXOpen.Point):
                c = g.Coordinates
                geo.append({'kind': 'point', 'name': g.Name, 'xyz': [c.X, c.Y, c.Z]})
            else:
                geo.append({'kind': type(g).__name__, 'name': g.Name})
        out['sketches'].append({'name': sk.Name, 'feature': sk.Feature.GetFeatureName() if sk.Feature else '', 'geometry': geo})
    out['bodies'] = [{'name': b.Name, 'journal': b.JournalIdentifier} for b in part.Bodies]
except Exception:
    out['errors'].append(traceback.format_exc())
with open(out_path, 'w', encoding='utf-8') as f:
    json.dump(out, f, indent=1)
print('DUMP_DONE', out_path, len(out.get('points', [])), 'points', len(out.get('lines', [])), 'lines', len(out.get('expressions', [])), 'expressions', len(out['errors']), 'errors')
