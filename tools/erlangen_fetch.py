# Downloads the open data for the whole city of Erlangen into data\erlangen (git-ignored); safe to run again, it skips what it has.
#   - terrain: Bavarian DGM1 (1 m grid, 1 km tiles) for every tile touching the city, from the survey office's list for Erlangen
#     (09562000), and the tiles in the corners of that box
#   - buildings: Bavarian LoD2 (CityGML, 2 km tiles), same list
#     both (c) Bayerische Vermessungsverwaltung, www.geodaten.bayern.de, CC BY 4.0
#   - map: Geofabrik's OpenStreetMap extract of Middle Franconia; (c) OpenStreetMap contributors, ODbL 1.0
#   python tools\erlangen_fetch.py
import hashlib
import os
import re
import subprocess
import sys
import urllib.request

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DATA = os.path.join(ROOT, 'data', 'erlangen')
LISTS = {
    'dgm1': 'https://geodaten.bayern.de/odd/a/dgm/dgm1/meta/metalink/09562000.meta4',
    'lod2': 'https://geodaten.bayern.de/odd/a/lod2/citygml/meta/metalink/09562000.meta4',
}
OSM = 'https://download.geofabrik.de/europe/germany/bayern/mittelfranken-latest.osm.pbf'


def get(url, path):
    tmp = path + '.part'   # curl (part of Windows): Python's own client timed out on Geofabrik's redirect
    subprocess.run(['curl', '-L', '--fail', '-s', '-S', '--retry', '3', '-C', '-', '-o', tmp, url], check=True)
    os.replace(tmp, path)


def sha256(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        while chunk := f.read(1 << 20):
            h.update(chunk)
    return h.hexdigest()


def entries(meta4):
    for name, size, digest, urls in re.findall(r'<file name="([^"]+)">\s*<size>(\d+)</size>\s*<hash type="sha-256">([0-9a-f]+)</hash>((?:\s*<url>[^<]+</url>)+)', meta4):
        yield name, int(size), digest, re.findall(r'<url>([^<]+)</url>', urls)


def fetch_list(kind, url):
    folder = os.path.join(DATA, kind)
    os.makedirs(folder, exist_ok=True)
    meta = urllib.request.urlopen(url, timeout=60).read().decode('utf-8')
    files = list(entries(meta))
    assert files and len(files) == meta.count('<file '), (kind, 'metalink not understood')
    for i, (name, size, digest, urls) in enumerate(files):
        path = os.path.join(folder, name)
        if os.path.exists(path) and os.path.getsize(path) == size:
            continue
        for u in urls:   # two mirrors
            try:
                get(u, path)
                break
            except (OSError, subprocess.CalledProcessError) as e:
                print('  retry', name, e)
        assert sha256(path) == digest, (name, 'checksum differs')
        print(f'{kind} {i + 1}/{len(files)} {name}', flush=True)
    print(kind, len(files), 'files ok', flush=True)
    return files


def fill_box(folder):
    """The terrain tiles in the corners of the city's box that its own list leaves out (they touch only its neighbours), so
    the ground beyond the boundary is real too. Fetched by the survey's tile naming; with no checksum, each must open
    as a 1000 x 1000 grid."""
    from PIL import Image
    have = {tuple(int(v) for v in f[:-4].split('_')) for f in os.listdir(folder) if f.endswith('.tif')}
    es, ns = [e for e, _ in have], [n for _, n in have]
    for e in range(min(es), max(es) + 1):
        for n in range(min(ns), max(ns) + 1):
            if (e, n) in have:
                continue
            path = os.path.join(folder, f'{e}_{n}.tif')
            get(f'https://download1.bayernwolke.de/a/dgm/dgm1/{e}_{n}.tif', path)
            assert Image.open(path).size == (1000, 1000), (path, 'not a terrain tile')
            print('dgm1 box', e, n, flush=True)


if __name__ == '__main__':
    for kind in sys.argv[1:] or ['dgm1', 'lod2', 'osm']:
        if kind == 'osm':
            os.makedirs(os.path.join(DATA, 'osm'), exist_ok=True)
            path = os.path.join(DATA, 'osm', 'mittelfranken.osm.pbf')
            if not os.path.exists(path):
                get(OSM, path)
            print('osm', os.path.getsize(path) // 1000000, 'MB', flush=True)
        else:
            fetch_list(kind, LISTS[kind])
            if kind == 'dgm1':
                fill_box(os.path.join(DATA, kind))
