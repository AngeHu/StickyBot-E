# Sticker-Applikator – CAD

## Ordner
| Ordner | Inhalt |
|---|---|
| `coupon/` | Toleranz-Coupon (`toleranz_coupon_v1.stl`) + Generator `coupon.py` |
| `ref/` | Fremdmodelle, z. B. `ST3215.step` |
| `print/` | Druckfertige 3MF-Platten aus Bambu Studio |
| (hier) | Fusion-Archiv + STEP je Version |

## Dateinamen
`applikator_v<N>.f3d` · `applikator_v<N>.step` · `print/applikator_v<N>_platte.3mf`
Eine Version = ein Commit: `applikator: v<N> <was>` (z. B. `applikator: v1 cad`).
Fusion: Datei → Exportieren → `.f3d` (Archiv) und `.step`. Nie nur in der Fusion-Cloud lassen.

## Parameter (Fusion → Ändern → Parameter ändern)
| Name | Wert | Herkunft |
|---|---|---|
| tol | 0,2 mm | nach Coupon anpassen |
| wand | 3 mm | |
| m3 | 3,4 mm | nach Coupon anpassen |
| m3_mutter | 6,0 mm (SW) | nach Coupon anpassen |
| servo_l | 45,2 mm | Datenblatt, messen |
| servo_b | 24,7 mm | Datenblatt, messen |
| servo_h | 35 mm | Datenblatt, messen |
| traeger_b | – | messen |
| rolle_d | – | messen |
| sticker_hoehe | – | mit Team festlegen |

## Toleranz-Coupon v1 (Lesehilfe)
60 × 105,6 × 4 mm, PLA, ca. 11 cm³, keine Stützen, flach drucken.
- 3 Servo-Rahmen (Servo von oben einstecken): Spiel pro Seite 1 Punkt = 0,15 · 2 Punkte = 0,25 · 3 Punkte = 0,35 mm
- Durchgangslöcher (links → rechts, 1/2/3 Punkte): Ø 3,2 · 3,4 · 3,6 mm
- M3-Mutter-Taschen (1/2/3 Punkte): Schlüsselweite 5,8 · 6,0 · 6,2 mm, 2,6 mm tief
Ergebnis: jeweils kleinste Variante, die ohne Gewalt passt → in Parameter übernehmen und in `docs/applikator/log.md` → Druck-Log.
