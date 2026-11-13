# make_jobs.py: from out\tree.json, the unique parts to export and the file in the copy for each (no NX needed).
# Skips standard fasteners, skeletons and the livery body (the game already has its own livery model).
import collections
import json
import os
import re

ROOT = r'D:\FSLap_CAD\Gesamtmodell'
tree = json.load(open(r'D:\FSLap_CAD\out\tree.json'))
files = collections.defaultdict(list)
for folder, _, names in os.walk(ROOT):
    for n in names:
        if n.lower().endswith('.prt'):
            files[n[:-4].lower()].append(os.path.join(folder, n))

SKIP = re.compile(r'^(din|iso|en|dsc|nas|nut|screw|washer)|skel|livery|^jacks$', re.I)


def pick(paths):
    """prefer files outside the old-version folders (alt, alt v2 ...), then the shortest path"""
    current = [p for p in paths if not re.search(r'[\\/]alt( v\d)?[\\/]', p, re.I)] or paths
    return sorted(current, key=len)[0]


jobs, missing, ambiguous = {}, [], []
for c in tree['components']:
    name = c['name']
    if c['suppressed'] or SKIP.search(name) or name.lower() in jobs:
        continue
    found = files.get(name.lower())
    if not found:
        missing.append(name)
        continue
    if len(found) > 1:
        ambiguous.append({'name': name, 'files': found, 'picked': pick(found)})
    jobs[name.lower()] = pick(found)
json.dump({'jobs': jobs, 'missing': sorted(set(missing)), 'ambiguous': ambiguous}, open(r'D:\FSLap_CAD\out\jobs.json', 'w'), indent=1)
print(len(jobs), 'parts to export,', len(set(missing)), 'names without a file,', len(ambiguous), 'picked among duplicates')

# self-check of the duplicate rule
assert pick([r'D:\a\alt v3\x.prt', r'D:\a\x.prt']) == r'D:\a\x.prt'
assert pick([r'D:\a\alt\x.prt']) == r'D:\a\alt\x.prt'
