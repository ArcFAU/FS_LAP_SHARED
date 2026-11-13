# hardpoints.py: the named suspension points of S_0000_SKEL (out\s_skel.json) as a clean table, left side, mm.
# Car frame of the CAD: x forward from the rear axle, y to the left, z up from the ground (ISO 8855).
import csv
import json

MEANING = {
    'COG': 'centre of gravity', 'FWC': 'front wheel centre', 'FCP': 'front contact patch', 'RWC': 'rear wheel centre', 'RCP': 'rear contact patch',
    'FUF': 'front upper wishbone, inner front', 'FUR': 'front upper wishbone, inner rear', 'FUO': 'front upper wishbone, outer (ball joint)',
    'FLF': 'front lower wishbone, inner front', 'FLR': 'front lower wishbone, inner rear', 'FLO': 'front lower wishbone, outer (ball joint)',
    'FTRI': 'front tie rod, inner', 'FTRO': 'front tie rod, outer',
    'FPRI': 'front pushrod, at the rocker', 'FPRO': 'front pushrod, at the wishbone', 'FRP': 'front rocker pivot',
    'FDI': 'front damper, inner (across the centre line: the dampers lie across the car)', 'FDO': 'front damper, at the rocker',
    'RUF': 'rear upper wishbone, inner front', 'RUR': 'rear upper wishbone, inner rear', 'RUO': 'rear upper wishbone, outer (ball joint)',
    'RLF': 'rear lower wishbone, inner front', 'RLR': 'rear lower wishbone, inner rear', 'RLO': 'rear lower wishbone, outer (ball joint)',
    'RTRI': 'rear toe link, inner', 'RTRO': 'rear toe link, outer',
    'RPRI': 'rear pushrod, at the rocker', 'RPRO': 'rear pushrod, at the wishbone', 'RRP': 'rear rocker pivot',
    'RDI': 'rear damper, inner (across the centre line)', 'RDO': 'rear damper, at the rocker',
    'RRARB': 'rear anti-roll bar, at the rocker', 'RTARB': 'rear anti-roll bar, drop link top', 'RLARB': 'rear anti-roll bar, lever',
}
skel = json.load(open(r'D:\FSLap_CAD\out\s_skel.json'))
points = {}
for p in skel['points']:
    name = p['name']
    if name in MEANING and name not in points:
        points[name] = [round(v, 2) for v in p['xyz']]
missing = sorted(set(MEANING) - set(points))
# the side every point is on: everything but the centre line must be on the left (y > 0)
assert all(points[n][1] > 0 for n in points if n not in ('COG', 'FDI', 'RDI')), points
out = {'source': 'S_0000_SUSPENSION/S_0000_SKEL.prt of 000_EV_CAD_Gesamtmodell (2026 car, FAUmax tau), read from a copy',
       'frame': 'mm; x forward from the rear axle, y left, z up from the ground; right side = y mirrored',
       'points': {n: {'xyz': points[n], 'meaning': MEANING[n]} for n in MEANING if n in points}, 'not_found': missing,
       'design_table': 'S_0000_SUSPENSION/TAU_SKEL_Import.xlsx (copied to out\\TAU_SKEL_Import.csv)'}
json.dump(out, open(r'D:\FSLap_CAD\out\hardpoints.json', 'w'), indent=1)
with open(r'D:\FSLap_CAD\out\hardpoints.csv', 'w', newline='') as f:
    w = csv.writer(f)
    w.writerow(['name', 'x_mm', 'y_mm', 'z_mm', 'meaning'])
    for n, v in out['points'].items():
        w.writerow([n, *v['xyz'], v['meaning']])
print(len(out['points']), 'points;', 'not found:', missing)
