# make_flat.py: D:\FSLap_CAD\flat holds every part file of the copy once, as a hard link (no extra disk space).
# NX can then load the whole assembly from one folder: the copy has 117 names twice (old versions in alt* folders),
# which makes NX's subfolder search fail. Only links into the copy; the SVN checkout is never touched.
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
ROOT, FLAT = r'D:\FSLap_CAD\Gesamtmodell', r'D:\FSLap_CAD\flat'


def pick(paths):
    """prefer files outside the old-version folders (alt, alt v2 ...), then the shortest path"""
    current = [p for p in paths if not re.search(r'[\\/]alt( v\d)?[\\/]', p, re.I)] or paths
    return sorted(current, key=len)[0]


files = {}
for folder, _, names in os.walk(ROOT):
    for n in names:
        if n.lower().endswith('.prt'):
            files.setdefault(n.lower(), []).append(os.path.join(folder, n))
os.makedirs(FLAT, exist_ok=True)
made = 0
for name, paths in files.items():
    target = os.path.join(FLAT, os.path.basename(pick(paths)))
    if not os.path.exists(target):
        os.link(pick(paths), target)
        made += 1
assert all(os.path.realpath(p).lower().startswith('d:\\fslap_cad') for p in [FLAT, ROOT])
print(made, 'links made,', len(files), 'part names in', FLAT)
