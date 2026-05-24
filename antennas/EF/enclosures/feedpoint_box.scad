// ============================================================
//  FEEDPOINT BOX — L-Network Matching for EFQW HF Antennas
//  TM-EF-ENC-002 Rev A
//  Contains: series toroid, shunt NP0 cap, variable trimmer,
//            choke balun toroid (FT-37-43 or FT-240-31)
//  Connectors: SMA(F) coax in, binding post (antenna wire out)
//  Material: PETG; copper tape lining for RF shielding
//  Print bed: 200×200×200 mm
// ============================================================

// --- Main box dimensions ---
bx = 70;   // length (X)
by = 50;   // width (Y)
bz = 38;   // height (Z)
wt = 2.0;  // wall thickness
ft = 2.5;  // floor thickness
lt = 2.0;  // lid thickness

// --- SMA female panel mount (threaded collar) ---
sma_d   = 6.5;   // SMA hole diameter
sma_y   = by/2;  // centered in Y
sma_z   = 12;    // height on front face

// --- Binding post hole ---
bp_d    = 6.5;   // M6 banana jack
bp_y    = by/2;
bp_z    = 12;

// --- Trimmer access hole (top face) ---
trim_d  = 6.0;   // screwdriver slot
trim_x  = bx - 15;
trim_y  = by/2;

// --- LED indicator hole (front) ---
led_d   = 3.2;   // 3mm LED
led_y   = by*0.75;
led_z   = 22;

// --- Snap fit ---
snap_h  = 1.5;
snap_d  = 0.6;

$fn = 48;

// ============================================================
module box_body() {
    difference() {
        cube([bx, by, bz]);

        // hollow
        translate([wt, wt, ft])
            cube([bx - 2*wt, by - 2*wt, bz]);

        // front face: SMA hole
        translate([-1, sma_y, sma_z])
            rotate([0, 90, 0])
                cylinder(d=sma_d, h=wt+2);

        // front face: LED
        translate([-1, led_y, led_z])
            rotate([0, 90, 0])
                cylinder(d=led_d, h=wt+2);

        // rear face: binding post
        translate([bx - wt - 1, bp_y, bp_z])
            rotate([0, 90, 0])
                cylinder(d=bp_d, h=wt+2);

        // top: trimmer access
        translate([trim_x, trim_y, bz - lt - 1])
            cylinder(d=trim_d, h=lt+2);

        // snap groove
        translate([wt, wt, bz - snap_h])
            cube([bx - 2*wt, by - 2*wt, snap_h + 1]);
    }
}

// ============================================================
module component_mounts() {
    // Toroid standoff pillar (series inductor L1)
    translate([15, by/2, ft])
        cylinder(d=5, h=6);

    // Choke balun standoff pillar (FT-37-43 or FT-240-31)
    translate([40, by/2, ft])
        cylinder(d=8, h=6);

    // Cap board slot (NP0 + variable trimmer PCB rails)
    translate([55, wt + 3, ft])
        cube([2, by - 2*wt - 6, 8]);
    translate([55, by - wt - 5, ft])
        cube([2, 2, 8]);
}

// ============================================================
module lid() {
    translate([0, 0, bz]) {
        difference() {
            cube([bx, by, lt]);

            // snap skirt undercut
            translate([wt + snap_d, wt + snap_d, 0])
                cube([bx - 2*(wt+snap_d),
                      by  - 2*(wt+snap_d),
                      lt]);
        }
    }
}

// ============================================================
module label_top() {
    translate([bx/2, by/2, bz + lt])
        linear_extrude(0.4)
            text("EFQW TUNER", size=4.5, halign="center", valign="center",
                 font="Liberation Sans:style=Bold");
}

// ============================================================
// RENDER
difference() {
    union() {
        box_body();
        component_mounts();
    }
    // already subtracted in box_body
}

// Lid offset
translate([bx + 8, 0, 0]) {
    lid();
    label_top();
}

// ============================================================
// Internal wiring guide (embedded in model comments):
// SMA center → L1 series toroid input
// L1 output  → junction: C1 NP0 to GND, C2 trimmer to GND, binding post to ant wire
// Choke balun wound on FT-240-31: 8T RG-174, placed after SMA on coax line
// Copper tape: line inside walls before component install; leave floor clear for airflow
// GND: coax braid → SMA shell → copper tape → chassis (RF ground)
// Counterpoise wire: separate binding post or banana jack on side wall (optional)
// ============================================================
