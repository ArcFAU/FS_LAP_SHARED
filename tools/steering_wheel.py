# Generates data\car\SM_SteeringWheel.glb: the CAD model has no steering wheel, and the onboard view needs one.
# The rim lies in the plane normal to x with the pivot at the origin; glTF axes (x forward, y up, z right).
import os
import sys

import numpy as np

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import glb

OUT = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'data', 'car', 'SM_SteeringWheel.glb')


def torus(radius, tube, material, nu=48, nv=12):
    u, v = np.meshgrid(np.linspace(0, 2 * np.pi, nu + 1), np.linspace(0, 2 * np.pi, nv + 1), indexing='ij')
    n = np.stack([np.sin(v), np.cos(v) * np.cos(u), np.cos(v) * np.sin(u)], -1).reshape(-1, 3)
    p = n * tube + np.stack([np.zeros_like(u), radius * np.cos(u), radius * np.sin(u)], -1).reshape(-1, 3)
    idx = []
    for i in range(nu):
        for j in range(nv):
            a, b = i * (nv + 1) + j, (i + 1) * (nv + 1) + j
            idx += [a, b, a + 1, a + 1, b, b + 1]
    return material, p, n, np.zeros((len(p), 2)), idx


def box(cy, cz, hy, hz, hx, material, angle=0.0):
    """A flat bar in the wheel plane, centred at (cy, cz) and turned by angle."""
    ca, sa = np.cos(angle), np.sin(angle)
    points, normals, idx = [], [], []
    faces = [((-1, 0, 0), [(-1, -1, -1), (-1, 1, -1), (-1, -1, 1), (-1, 1, 1)]), ((1, 0, 0), [(1, -1, -1), (1, 1, -1), (1, -1, 1), (1, 1, 1)]),
             ((0, -1, 0), [(-1, -1, -1), (1, -1, -1), (-1, -1, 1), (1, -1, 1)]), ((0, 1, 0), [(-1, 1, -1), (1, 1, -1), (-1, 1, 1), (1, 1, 1)]),
             ((0, 0, -1), [(-1, -1, -1), (1, -1, -1), (-1, 1, -1), (1, 1, -1)]), ((0, 0, 1), [(-1, -1, 1), (1, -1, 1), (-1, 1, 1), (1, 1, 1)])]
    for normal, corners in faces:
        k = len(points)
        for sx, sy, sz in corners:
            y, z = sy * hy, sz * hz
            points.append((sx * hx, cy + y * ca - z * sa, cz + y * sa + z * ca))
            normals.append((normal[0], normal[1] * ca - normal[2] * sa, normal[1] * sa + normal[2] * ca))
        idx += [k, k + 1, k + 2, k + 1, k + 3, k + 2]
    return material, np.array(points, 'f8'), np.array(normals, 'f8'), np.zeros((len(points), 2)), idx


def main():
    radius, tube = 0.125, 0.016
    parts = [torus(radius, tube, 'Rubber'), box(0.0, 0.0, 0.05, 0.075, 0.014, 'Carbon')]
    for angle in (0.0, np.pi / 2 + 0.5, -np.pi / 2 - 0.5):   # three spokes
        direction = np.array([np.cos(angle), np.sin(angle)])
        middle = direction * (radius + 0.04) / 2
        parts.append(box(middle[0], middle[1], (radius - 0.04) / 2 + 0.01, 0.012, 0.008, 'Carbon', angle))
    glb.write(OUT, 'SM_SteeringWheel', parts)
    print('wrote', OUT)


if __name__ == '__main__':
    main()
