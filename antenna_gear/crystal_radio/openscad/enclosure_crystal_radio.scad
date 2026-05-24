/*
 * enclosure_crystal_radio.scad
 * Crystal Radio Enclosure with Variable Capacitor Mount
 * TM-CR-001 Rev A
 * Material: PETG or PLA+; non-conductive (no ABS near AM frequencies)
 * Print bed: 200 × 200 mm (fits in two parts)
 *
 * Contents:
 *   - Baseboard (open platform): ferrite rod brackets, cap mount, component layout
 *   - Front panel: binding posts, earphone jack, regen control
 *   - Vernier dial hub: 10:1 reduction drive for variable cap
 *   - Capacitor mount: 365 pF air-variable bracket + shaft coupling
 *   - Rod bracket: adjustable ferrite rod saddle (×2)
 *   - Optional lid/cover: open-frame style (decorative only)
 *
 * Design philosophy:
 *   Crystal radios are traditionally built on an open baseboard.
 *   This enclosure follows that tradition: a hardwood-style open platform
 *   with all components visible and accessible.  The PETG baseboard
 *   replaces walnut/cherry hardwood with equivalent appearance.
 *
 * Overall dimensions (baseboard): 200 × 120 × 12 mm
 * Component placement follows classic crystal radio layout:
 *   Left side: ferrite rod antenna (longitudinal)
 *   Center: variable capacitor + vernier dial
 *   Right side: detector, binding posts, earphone jack
 */

$fn = 64;

// ── Global dimensions ─────────────────────────────────────────────────────────
BB_L   = 200;    // Baseboard length
BB_W   = 120;    // Baseboard width
BB_H   = 12;     // Baseboard thickness (solid; heavy feel)
CORN_R = 8;      // Corner radius

// ── Utility ───────────────────────────────────────────────────────────────────
module rounded_plate(l, w, h, r) {
    hull()
        for (x=[r, l-r]) for (y=[r, w-r])
            translate([x, y, 0]) cylinder(r=r, h=h);
}

module countersink(d_top, d_bot, h) {
    cylinder(d=d_bot, h=h);
    translate([0,0,h-1]) cylinder(d1=d_bot, d2=d_top, h=1);
}

// ── 1. BASEBOARD ──────────────────────────────────────────────────────────────
/*
 * Solid PETG platform with:
 *   - M3 standoff holes for component PCBs
 *   - Ferrite rod slots (two saddle mount cutouts)
 *   - Variable capacitor mounting hole pattern
 *   - Rubber foot recesses (×4)
 *   - Decorative edge chamfer
 */

ROD_SADDLE_X1 = 25;    // First rod saddle X position (from left)
ROD_SADDLE_X2 = 85;    // Second rod saddle X position
ROD_Y         = 30;    // Rod saddle Y position (front third of board)
CAP_X         = 130;   // Variable capacitor X center
CAP_Y         = 55;    // Variable cap Y center
PANEL_X       = 175;   // Front panel X position (right side)

module baseboard() {
    difference() {
        union() {
            // Main plate
            rounded_plate(BB_L, BB_W, BB_H, CORN_R);
            // Edge bead (decorative)
            translate([0,0,BB_H-1])
                difference() {
                    rounded_plate(BB_L, BB_W, 1, CORN_R);
                    translate([4,4,-0.1]) rounded_plate(BB_L-8, BB_W-8, 1.2, CORN_R-4);
                }
        }

        // Rubber foot pockets (8 mm dia × 3 mm deep; ×4 corners)
        for (pos=[[12,12],[12,BB_W-12],[BB_L-12,12],[BB_L-12,BB_W-12]])
            translate([pos[0],pos[1],-0.1]) cylinder(d=8, h=3.2);

        // M3 standoff holes (×8; general purpose component mounting)
        for (pos=[[18,60],[18,90],[50,90],[100,90],[150,90],[100,60],[150,60],[170,90]])
            translate([pos[0],pos[1],0]) countersink(6, 3.4, BB_H+0.2);

        // Variable capacitor mounting holes (shaft bearing + 3×M3)
        // 365pF air-var: shaft center, mounting ears at ±22mm and ±15mm
        translate([CAP_X, CAP_Y, 0]) {
            // Shaft hole (7 mm diameter for 6 mm shaft + coupling)
            cylinder(d=7, h=BB_H+0.2);
            // Mounting screw holes (typical air-variable pattern)
            for (pos=[[-22, 15],[22, 15],[-22,-15],[22,-15]])
                translate([pos[0],pos[1],-0.1]) cylinder(d=3.4, h=BB_H+0.2);
        }

        // Ferrite rod slot at rod saddle positions (rod passes through saddle brackets)
        // Saddle brackets are separate parts; board has locating holes only
        for (x=[ROD_SADDLE_X1, ROD_SADDLE_X2])
            translate([x, ROD_Y, 0]) {
                cylinder(d=4, h=BB_H+0.2);                 // Center pin
                for (dx=[-8,8]) translate([dx,0,-0.1])
                    cylinder(d=3.4, h=BB_H+0.2);           // M3 mount
            }

        // Headphone jack hole (right edge, centered vertically)
        translate([BB_L-8, BB_W/2, BB_H/2])
            rotate([0,90,0]) cylinder(d=6.5, h=12);

        // Binding post holes (×2: ANT, GND)
        for (y=[BB_W/2+12, BB_W/2-12])
            translate([BB_L-8, y, BB_H/2])
                rotate([0,90,0]) cylinder(d=8.5, h=12);   // 5-way binding post OD

        // Label recess (engraved area for silk-screen or label)
        translate([4,4,BB_H-0.6]) rounded_plate(BB_L-8, BB_W-8, 0.7, CORN_R-4);
    }
}


// ── 2. VARIABLE CAPACITOR BRACKET ────────────────────────────────────────────
/*
 * Mounts a standard 365 pF broadcast air-variable capacitor to the baseboard.
 * Capacitor rests on platform raised 20 mm above baseboard.
 * Shaft exits toward front for knob/vernier dial attachment.
 * Threaded inserts or M3 screws into baseboard.
 *
 * Capacitor body assumed: 45 mm wide × 38 mm deep × 38 mm tall (typical)
 * Shaft center: 19 mm above bottom of cap
 */

CAP_BODY_W  = 48;    // Capacitor body clearance width
CAP_BODY_D  = 42;    // Depth (front-to-back)
CAP_BODY_H  = 42;    // Height (mounting surface to top)
CAP_SHAFT_H = 19;    // Shaft center height above capacitor base
CAP_RAISE   = 8;     // Height of pedestal above baseboard
CAP_SHAFT_Z = CAP_RAISE + CAP_SHAFT_H;  // Shaft Z above baseboard = 27 mm

module cap_bracket() {
    difference() {
        union() {
            // Pedestal (raised platform for cap body)
            translate([0, 0, 0])
                cube([CAP_BODY_W+8, CAP_BODY_D+8, CAP_RAISE], center=true);
            // Support ribs (fore/aft for rigidity)
            for (y=[-CAP_BODY_D/2, CAP_BODY_D/2])
                translate([0, y, CAP_RAISE/2])
                    cube([CAP_BODY_W+8, 3, CAP_BODY_H+CAP_RAISE], center=true);
            // Front lip to retain cap
            translate([0, -(CAP_BODY_D/2+2), CAP_BODY_H/2+CAP_RAISE/2])
                cube([CAP_BODY_W, 4, CAP_BODY_H], center=true);
        }
        // Interior pocket for cap body
        translate([0, 0, CAP_RAISE])
            cube([CAP_BODY_W, CAP_BODY_D, CAP_BODY_H+1], center=true);
        // Shaft clearance through front wall
        translate([0, -(CAP_BODY_D/2+2), CAP_SHAFT_H])
            rotate([90,0,0]) cylinder(d=8, h=8);
        // M3 mounting holes (×4, into baseboard)
        for (pos=[[-22,15],[22,15],[-22,-15],[22,-15]])
            translate([pos[0],pos[1],-CAP_RAISE/2-0.1])
                cylinder(d=3.4, h=CAP_RAISE+0.2);
        // Cap retention screw (top, M3, into cap body)
        translate([0, 0, CAP_BODY_H+CAP_RAISE-0.1]) cylinder(d=3.4, h=4);
    }
}


// ── 3. VERNIER DIAL HUB ───────────────────────────────────────────────────────
/*
 * 10:1 vernier reduction drive for the variable capacitor shaft.
 * Outer dial: 80 mm diameter, 0–100 scale (0.1 div = 1 pF resolution).
 * Inner drive: 8 mm hub couples to 6 mm cap shaft.
 *
 * The vernier drive uses a friction disk and a precision gear reduction
 * implemented via a 10:1 diameter ratio between drive wheel and dial ring.
 * This is a simplified direct-drive version (no gears).
 * For gear-reduction version, use commercial Velleman vernier drive.
 *
 * Printed version: direct dial with pointer; 1:1 scale, large 80 mm dia.
 * At 80 mm dial, 365 pF full range → 365 degrees of rotation ≈ 4.56 pF/degree.
 */

DIAL_OD   = 80;
DIAL_H    = 6;
HUB_D     = 12;    // Hub that couples to cap shaft
HUB_L     = 15;
SHAFT_D   = 6.0;   // Set screw retained
SET_D     = 3.2;   // M3 set screw

module vernier_dial() {
    difference() {
        union() {
            // Dial face
            cylinder(d=DIAL_OD, h=DIAL_H);
            // Hub
            translate([0,0,-HUB_L]) cylinder(d=HUB_D, h=HUB_L+DIAL_H);
        }
        // Shaft bore
        translate([0,0,-HUB_L-0.1])
            cylinder(d=SHAFT_D, h=HUB_L+DIAL_H+0.2);
        // Set screw hole (M3 × 1, perpendicular, at hub mid)
        translate([0, 0, -HUB_L/2])
            rotate([90,0,0]) cylinder(d=SET_D, h=HUB_D+0.2, center=true);
        // Scale markings: 10 major divisions (every 36°), 5 minor per major
        for (a=[0:5:355])
            rotate([0,0,a])
            translate([DIAL_OD/2-( (a%10==0) ? 4 : (a%5==0) ? 2.5 : 1.5),0,DIAL_H-0.8])
                cube([(a%10==0) ? 1.2 : 0.8, 0.6, 0.9]);
        // Center depression (decorative)
        cylinder(d=HUB_D+4, h=1.5);
        // Knob grip (6 flats on outer edge)
        for (a=[0:60:300])
            rotate([0,0,a+30]) translate([DIAL_OD/2-1, 0, DIAL_H/2])
                cube([2, 3, DIAL_H+0.2], center=true);
    }
    // Pointer indicator (separate piece; glue to baseboard or front panel)
    translate([DIAL_OD/2+1, 0, 0])
        translate([0,0,DIAL_H/2]) cube([6,1.2,DIAL_H*0.8], center=true);
}


// ── 4. FERRITE ROD SADDLE ─────────────────────────────────────────────────────
/*
 * V-groove saddle that cradles the 9.5 mm ferrite rod.
 * Two saddles per rod, spaced to allow coil adjustment.
 * Rod is held by a snap-over cap retained with M2 screw.
 * Low mounting: rod center 8 mm above baseboard surface.
 */

ROD_D    = 9.5;
SADDLE_W = 16;
SADDLE_H = 12;
SADDLE_D = 18;

module rod_saddle() {
    difference() {
        union() {
            // Main saddle body
            cube([SADDLE_W, SADDLE_D, SADDLE_H]);
            // Snap-over cap retainer boss
            translate([SADDLE_W/2, SADDLE_D/2, SADDLE_H])
                cylinder(d=SADDLE_W-2, h=3);
        }
        // V-groove for rod (60° V, sized for 9.5 mm rod)
        translate([SADDLE_W/2, SADDLE_D/2, SADDLE_H+1])
            cylinder(d=ROD_D+0.5, h=8, center=true);
        // Base mounting holes (×2 M3)
        for (y=[4, SADDLE_D-4])
            translate([SADDLE_W/2, y, -0.1]) cylinder(d=3.4, h=4.2);
        // Wire routing notch (for coil leads)
        translate([SADDLE_W/2, SADDLE_D-0.1, 4])
            cube([3, 2, 6], center=true);
    }
}

module rod_saddle_cap() {
    difference() {
        cylinder(d=SADDLE_W-2, h=4);
        // Rod channel
        translate([0,0,-0.1]) cylinder(d=ROD_D+0.6, h=4.2);
        // Retention screw (M2)
        translate([0, -(SADDLE_W/2-2), 2])
            rotate([90,0,0]) cylinder(d=2.2, h=SADDLE_W, center=true);
    }
}


// ── 5. DETECTOR BOARD STANDOFF ────────────────────────────────────────────────
/*
 * Small standoff post (×4) for mounting a 50×30 mm detector PCB or
 * tagboard above the baseboard.  Accepts M3 screw from above.
 * Height: 10 mm (adequate for solder joints on underside of board).
 */

module pcb_standoff(h=10) {
    difference() {
        cylinder(d=7, h=h);
        translate([0,0,-0.1]) cylinder(d=3.0, h=h+0.2);  // M3 thread bore
    }
}


// ── 6. REGEN CONTROL KNOB ─────────────────────────────────────────────────────
/*
 * Knob for 100 kΩ regeneration potentiometer (6 mm D-shaft).
 * Diameter: 28 mm (easy to grip).
 * Pointer line cast into face.
 */

module regen_knob() {
    difference() {
        union() {
            cylinder(d=28, h=12);
            // Skirt
            translate([0,0,0]) cylinder(d1=30, d2=28, h=2);
        }
        // D-shaft bore (6mm + flat)
        translate([0,0,-0.1]) {
            cylinder(d=6.1, h=8);
            translate([-3.1, 6.1/2-1.0, 0]) cube([6.2, 2, 8]);
        }
        // Set screw M3 (optional)
        translate([0, 14.5, 4]) rotate([90,0,0]) cylinder(d=3.2, h=12);
        // Pointer line on top
        translate([-0.5, 0, 11]) cube([1, 14, 1.2]);
        // Grip flutes (×10 around circumference)
        for (a=[0:36:324])
            rotate([0,0,a]) translate([14, 0, 6])
                cylinder(d=2.5, h=8, center=true);
    }
}


// ── RENDER ─────────────────────────────────────────────────────────────────────
// Print baseboard flat on bed:
baseboard();

// Print dial separately:
// translate([0, BB_W+10, 0]) vernier_dial();

// Print saddles (×2):
// translate([BB_L+10, 0, 0]) rod_saddle();
// translate([BB_L+10, SADDLE_D+5, 0]) rod_saddle();
// translate([BB_L+10, 2*(SADDLE_D+5), 0]) rod_saddle_cap();
// translate([BB_L+10, 2*(SADDLE_D+5)+20, 0]) rod_saddle_cap();

// Print cap bracket:
// translate([BB_L+15, 60, 0]) cap_bracket();

// Regen knob:
// translate([BB_L+50, 0, 0]) regen_knob();

// PCB standoffs (print ×4):
// for (i=[0:3]) translate([BB_L+10+i*12, BB_W-20, 0]) pcb_standoff();
