# Toleranz-Coupon fuer den Sticker-Applikator. Ausfuehren: pip install manifold3d trimesh && python coupon.py
import math, numpy as np, trimesh
from manifold3d import Manifold, set_circular_segments
set_circular_segments(64)
X, Z = 60.0, 4.0
W = 3.0
CL = [0.15, 0.25, 0.35]        # Spiel pro Seite
SL, SB = 45.2, 24.7            # ST3215 Grundfläche laut Datenblatt
strip = 18.0
Y = W + sum(SB+2*c+W for c in CL) + strip
body = Manifold.cube([X, Y, Z])
bumps = []
def bump(x,y): bumps.append(Manifold.cylinder(0.6, 0.8).translate([x,y,Z]))
y = W
for i,c in enumerate(CL, start=1):
    px, py = SL+2*c, SB+2*c
    x0 = (X-px)/2
    body = body - Manifold.cube([px, py, Z+2]).translate([x0, y, -1])
    yc = y + py/2
    for k in range(i): bump(x0/2, yc + (k-(i-1)/2)*3)
    y += py + W
ys = y + strip/2
for i,(d,x) in enumerate(zip([3.2,3.4,3.6],[7,15,23]), start=1):
    body = body - Manifold.cylinder(Z+2, d/2).translate([x, ys, -1])
    for k in range(i): bump(x + (k-(i-1)/2)*2.4, ys+6)
for i,(af,x) in enumerate(zip([5.8,6.0,6.2],[33,43,53]), start=1):
    r = af/math.sqrt(3)
    body = body - Manifold.cylinder(2.6+1, r, r, 6).translate([x, ys, Z-2.6])
    body = body - Manifold.cylinder(Z+2, 3.4/2).translate([x, ys, -1])
    for k in range(i): bump(x + (k-(i-1)/2)*2.4, ys+6)
for b in bumps: body = body + b
m = body.to_mesh()
mesh = trimesh.Trimesh(vertices=np.asarray(m.vert_properties)[:, :3], faces=np.asarray(m.tri_verts))
out='toleranz_coupon_v1.stl'
mesh.export(out)
print('watertight',mesh.is_watertight,'bounds',mesh.bounds.round(2).tolist(),'vol cm3',round(mesh.volume/1000,2))
