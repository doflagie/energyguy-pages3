// TM-VCAP-ENC-006 Rev A — Sliding PVC Pipe Variable Capacitor Assembly
// Plunger-type coaxial pipe cap: outer sleeve end cap, guide bushing,
// thumb knob, position scale, and optional NEMA14 motor mount.
//
// Companion to:  TM-VCAP-SCH-004, TM-VCAP-CAL-005
// Print material: PETG (outer sleeve) or PLA (thumb knob, end caps)
// Recommended infill: 40%+ for end caps; 60%+ for motor mount bracket

// ─── Parameters ────────────────────────────────────────────────────────────────
// Inner pipe: 1/2" Schedule 40 PVC OD = 21.34 mm
// Dielectric (2× Kapton 0.065mm): total 0.13 mm → covered OD ≈ 21.60 mm
// Copper foil tape (0.10mm) under dielectric → copper OD ≈ 21.54 mm
// Sleeve ID: 22.00 mm (0.40mm total clearance — snug but slidable)

INNER_OD        = 21.34;   // PVC pipe OD (mm) — 1/2" Sch 40
FOIL_T          = 0.10;    // Copper foil tape thickness (mm)
DIEL_T          = 0.13;    // Dielectric total (2× Kapton)
SLIDE_CLEARANCE = 0.20;    // Radial clearance between dielectric and sleeve ID
SLEEVE_ID       = INNER_OD + 2*(FOIL_T + DIEL_T) + SLIDE_CLEARANCE; // ≈ 22.0 mm
SLEEVE_WALL     = 3.0;     // Outer sleeve wall thickness (mm) — PETG
SLEEVE_OD       = SLEEVE_ID + 2*SLEEVE_WALL;    // ≈ 28.0 mm
SLEEVE_LEN      = 170;     // Outer sleeve body length (mm) — covers 0..150mm travel + ends

MAX_DEPTH       = 150;     // Maximum insertion depth (mm) — PIPE-HF
KNOB_OD         = 35;      // Thumb knob outer diameter (mm)
KNOB_LEN        = 22;      // Thumb knob length (mm)
KNOB_BORE       = INNER_OD + 0.4;  // Light press fit on inner pipe end

ENDCAP_T        = 6;       // End-cap wall thickness (mm)
ENDCAP_OD       = SLEEVE_OD + 6;   // End cap OD — gives lip for chassis mount

LEAD_SCREW_DIA  = 6.0;     // M6 threaded rod (mm)
NUT_HEX_W       = 10.0;    // M6 nut across-flats (mm)
NUT_HEX_H       = 5.0;     // M6 nut height (mm)

NEMA14_BOLT_SEP = 26.0;    // NEMA14 bolt circle across (mm)
NEMA14_BOSS_DIA = 22.0;    // NEMA14 pilot boss diameter (mm)
NEMA14_SHAFT_DIA= 5.0;     // NEMA14 shaft diameter (mm)
NEMA14_FACEPLATE_T = 4.0;  // Motor face-plate thickness (mm)

$fn = 72;

// ─── Modules ───────────────────────────────────────────────────────────────────

module outer_sleeve() {
    // Hollow cylinder: holds outer copper foil inside; inner pipe slides through.
    // In practice the foil is applied to the bore before printing; this is the
    // structural sleeve. Print with inner surface lightly sanded (220 grit).
    difference() {
        cylinder(h=SLEEVE_LEN, d=SLEEVE_OD);
        // Bore
        cylinder(h=SLEEVE_LEN, d=SLEEVE_ID);
        // Foil-access slot on the fixed end — 4mm×15mm rectangular cut for
        // outer conductor terminal wire routing
        translate([SLEEVE_OD/2-SLEEVE_WALL+0.5, -2, SLEEVE_LEN-15])
            cube([SLEEVE_WALL+1, 4, 15]);
    }
    // Position scale rail: raised 1mm ridge along outer surface (0°), width 2mm
    // Laser-engrave or cut mm/pF graduation marks onto this ridge after printing.
    translate([0, SLEEVE_OD/2, 10])
        rotate([-90,0,0])
        translate([-1, 0, 0])
        cube([2, SLEEVE_LEN-20, 1.2]);
}

module endcap_fixed() {
    // Fixed end cap (far end from thumb knob / motor end).
    // Bolts to chassis. T₂ (outer conductor) terminal exits here.
    difference() {
        union() {
            // Main cap body
            cylinder(h=ENDCAP_T, d=ENDCAP_OD);
            // Bearing boss for inner pipe (keeps pipe centred)
            cylinder(h=ENDCAP_T+8, d=INNER_OD+3*2+2);
        }
        // Inner pipe bore through cap
        cylinder(h=ENDCAP_T+10, d=INNER_OD+0.5);
        // Sleeve pocket (cap sits over sleeve end)
        translate([0,0,ENDCAP_T-3])
            cylinder(h=4, d=SLEEVE_OD+0.4);
        // T₂ terminal hole (M3 through, for wire soldered to outer foil tab)
        translate([ENDCAP_OD/2-5, 0, ENDCAP_T/2])
            rotate([0,90,0]) cylinder(h=8, d=3.2);
        // Four M3 chassis mounting holes at 90° on flange
        for (a=[0,90,180,270])
            rotate([0,0,a]) translate([ENDCAP_OD/2-4, 0, ENDCAP_T/2])
                rotate([0,90,0]) cylinder(h=8, d=3.4);
    }
}

module endcap_guide() {
    // Open guide bushing (plunger end) — centres inner pipe as it moves.
    // Low friction: apply 3 PTFE tape pads to inner pipe OD at 120° before inserting.
    difference() {
        union() {
            cylinder(h=ENDCAP_T, d=ENDCAP_OD);
            cylinder(h=ENDCAP_T+4, d=INNER_OD+3*2+2);
        }
        // Inner pipe guide bore — loose fit to allow axial motion
        cylinder(h=ENDCAP_T+6, d=INNER_OD+0.8);
        // Sleeve pocket
        translate([0,0,ENDCAP_T-3])
            cylinder(h=4, d=SLEEVE_OD+0.4);
        // Vent holes (prevent air compression during insertion)
        for (a=[0,120,240])
            rotate([0,0,a]) translate([SLEEVE_ID/2-2, 0, 1])
                cylinder(h=ENDCAP_T+2, d=3.0);
        // Scale window — see position marks on inner pipe through this slot
        translate([-1.5, INNER_OD/2+0.5, 0])
            cube([3, 6, ENDCAP_T+4]);
    }
}

module thumb_knob() {
    // Friction-fit thumb knob on exposed end of inner pipe.
    // Contains hex nut pocket for M6 set screw (optional position lock).
    difference() {
        union() {
            // Knurled grip cylinder (faceted polygon approximates knurl)
            cylinder(h=KNOB_LEN, d=KNOB_OD, $fn=24);
            // Flange washer to act as depth stop against guide bushing
            cylinder(h=3, d=KNOB_OD+6, $fn=48);
        }
        // Inner pipe bore — light interference fit; heat to install
        cylinder(h=KNOB_LEN+1, d=KNOB_BORE);
        // M6 nut pocket for set screw (90° from grip)
        translate([KNOB_OD/2-NUT_HEX_H, 0, KNOB_LEN/2])
            rotate([0,90,0]) cylinder(h=NUT_HEX_H+2, d=NUT_HEX_W+0.4, $fn=6);
        // Set-screw bore M3 through to centre
        translate([0, 0, KNOB_LEN/2])
            rotate([0,90,0]) cylinder(h=KNOB_OD/2+2, d=3.2);
        // T₁ terminal wire exit: axial slot in bore (wire goes to inner foil tab)
        translate([-1.5, -KNOB_BORE/2-0.5, -0.5])
            cube([3, KNOB_BORE/2+2, KNOB_LEN+2]);
    }
}

module nut_adapter_inner() {
    // 3D-printed adapter: glues/presses into inner pipe end; has M6 nut pocket.
    // Motor turns M6 rod; this nut travels axially, pushing inner pipe.
    ADAPTER_OD = INNER_OD - 1.5;  // Slight press fit into pipe bore
    difference() {
        cylinder(h=18, d=ADAPTER_OD);
        // M6 nut pocket (axial, recessed 5mm from end)
        translate([0, 0, 5])
            cylinder(h=NUT_HEX_H+1, d=NUT_HEX_W+0.5, $fn=6);
        // Lead screw bore through (allows rod to extend past nut)
        cylinder(h=19, d=LEAD_SCREW_DIA+0.4);
    }
}

module motor_mount_plate() {
    // NEMA14 motor face plate; mounts to fixed end cap via M3 standoffs.
    // Standoffs: 25mm (end cap ENDCAP_T + inner distance to cap face).
    MT   = NEMA14_FACEPLATE_T;
    W    = NEMA14_BOLT_SEP + 12;  // Plate width
    H    = NEMA14_BOLT_SEP + 12;  // Plate height
    difference() {
        // Face plate
        translate([-W/2, -H/2, 0]) cube([W, H, MT]);
        // Motor pilot boss bore
        cylinder(h=MT+1, d=NEMA14_BOSS_DIA+0.6);
        // Motor shaft bore
        cylinder(h=MT+1, d=NEMA14_SHAFT_DIA+0.4);
        // Motor bolt holes (NEMA14: 26mm square pattern)
        for (sx=[-1,1]) for (sy=[-1,1])
            translate([sx*NEMA14_BOLT_SEP/2, sy*NEMA14_BOLT_SEP/2, -0.5])
                cylinder(h=MT+2, d=3.2);
        // Lead screw clear bore (centred on plate = centred on motor shaft)
        // (motor shaft IS the lead screw in direct-drive config; bore already above)
    }
    // Standoff posts (×4) extending back to end cap (length to suit chassis spacing)
    STANDOFF_L = 20;
    for (sx=[-1,1]) for (sy=[-1,1])
        translate([sx*(W/2-5), sy*(H/2-5), -STANDOFF_L])
            difference() {
                cylinder(h=STANDOFF_L, d=7);
                cylinder(h=STANDOFF_L+1, d=3.4);
            }
}

module scale_plate() {
    // Flat label plate to glue onto the outer sleeve scale rail.
    // Graduation marks and pF labels: engrave or print on paper and laminate.
    // PIPE-HF: K = 17.7 pF/mm; marks at 0,10,20,30,50,75,100,125,150mm
    //          pF:              0, 177, 354, 531, 885,1328,1770,2213,2655
    W = 6;
    difference() {
        cube([W, MAX_DEPTH, 1.5]);
        // Major graduation notches at 10mm intervals
        for (x=[0:10:MAX_DEPTH])
            translate([0, x-0.5, 0])
                cube([W, 1, 1.6]);
        // Minor graduation notches at 5mm intervals (shallower)
        for (x=[5:10:MAX_DEPTH])
            translate([1.5, x-0.3, 0])
                cube([W-3, 0.6, 1.6]);
    }
}

// ─── Assembly ──────────────────────────────────────────────────────────────────
// Print each part separately; explode for visual reference only.

// Outer sleeve (body)
translate([0, 0, 0])
    outer_sleeve();

// Fixed end cap (soldered T₂ terminal exits here; bore keeps inner pipe centred)
translate([50, 0, 0])
    endcap_fixed();

// Guide bushing (open end; inner pipe exits here with thumb knob)
translate([100, 0, 0])
    endcap_guide();

// Thumb knob (manual version)
translate([0, 50, 0])
    thumb_knob();

// M6 nut adapter (motorized version — insert into inner pipe end)
translate([50, 50, 0])
    nut_adapter_inner();

// NEMA14 motor mount plate (motorized version)
translate([100, 50, 0])
    motor_mount_plate();

// Position scale plate (glue to sleeve rail; engrave pF marks)
translate([0, 100, 0])
    scale_plate();
