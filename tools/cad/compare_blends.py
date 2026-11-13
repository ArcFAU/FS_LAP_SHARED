# compare_blends.py: which objects the owner's cleaned .blend dropped, grouped by the CAD subassembly they belong to.
import collections
import json

full = {o['name']: o for o in json.load(open(r'D:\FSLap_CAD\out\objects_faumax_tau_cad.json'))}
clean = {o['name']: o for o in json.load(open(r'D:\FSLap_CAD\out\objects_faumax_tau_cad_clean.json'))}
tree = json.load(open(r'D:\FSLap_CAD\out\tree.json'))['components']

# each component's path of parents, so a removed part can be named by its subsystem
path, stack = {}, []
for c in tree:
    stack = stack[:c['level'] - 1] + [c['name']]
    path.setdefault(c['name'], ' / '.join(stack[:-1]))

base = lambda n: n.split('.')[0]   # Blender adds .001 for repeated instances
removed = [full[n] for n in full if n not in clean and full[n]['type'] == 'MESH' and not n.startswith('HP_')]
added = [clean[n] for n in clean if n not in full]
hp_removed = [n for n in full if n.startswith('HP_') and n not in clean]
hidden = [n for n, o in clean.items() if o.get('hidden')]
print('objects: full', len(full), 'clean', len(clean), '| removed meshes', len(removed), '| hardpoint markers removed', len(hp_removed), '| added', len(added), '| hidden in clean', len(hidden))
print('triangles: full', sum(o.get('tris', 0) for o in full.values()), 'clean', sum(o.get('tris', 0) for o in clean.values()))

groups = collections.defaultdict(list)
for o in removed:
    p = path.get(base(o['name']), '?')
    key = ' / '.join(p.split(' / ')[:3]) if p else '(top level)'
    groups[key].append(o)
for key, items in sorted(groups.items(), key=lambda kv: -sum(o['tris'] for o in kv[1])):
    names = collections.Counter(base(o['name']) for o in items)
    print(f'\n{key}: {len(items)} objects, {sum(o["tris"] for o in items)} tris')
    print('   ', ', '.join(f'{n} x{k}' if k > 1 else n for n, k in names.most_common(25)) + (' ...' if len(names) > 25 else ''))
print('\nadded:', [o['name'] for o in added][:20])
print('hidden:', hidden[:20])
