// ============================================================
// COAXIAL TRAP ASSEMBLY — PARAMETRIC MULTI-BAND SET
// Generates complete trap dimensions for all HF bands
// Renders a visual assembly of former + enclosure
//
// USE: Set band_index to select which band to preview.
// Run trap_calculator.py to get exact winding data first.
// ============================================================

/* [Select Band] */
// 0=160M 1=80M 2=40M 3=30M 4=20M 5=17M 6=15M 7=12M 8=10M 9=6M
band_index = 2;  // default: 40M

/* [Coax Type] */
// 0=RG-213  1=RG-8X  2=RG-58
coax_index = 0;

$fn = 64;

// ============================================================
// Band/coax parameter tables (from trap_calculator.py output)
// [band, f_MHz, former_od_mm, turns, L_uH, C_pF, coax_len_m, Q]
// ============================================================
band_data = [
//  [band,  f_MHz, RG213_od, RG213_N, RG8X_od, RG8X_N, RG58_od, RG58_N]
    ["160M", 1.850, 120, 14.0, 100, 16.5, 80, 18.5],
    ["80M",  3.750,  90, 11.0,  76, 12.5, 60, 14.0],
    ["40M",  7.150,  76,  7.5,  60,  9.0, 50, 10.0],
    ["30M", 10.125,  60,  7.5,  50,  8.5, 40,  9.5],
    ["20M", 14.175,  50,  7.0,  40,  8.0, 35,  8.5],
    ["17M", 18.118,  45,  6.0,  35,  8.0, 30,  8.0],
    ["15M", 21.225,  40,  6.0,  32,  7.5, 25,  9.0],
    ["12M", 24.940,  35,  6.5,  28,  7.5, 22,  9.0],
    ["10M", 28.500,  30,  7.0,  25,  8.0, 20,  9.0],
    ["6M",  51.000,  25,  5.0,  20,  6.0, 16,  7.0],
];

coax_data = [
//  [name, od_mm, wall_typical]
    ["RG-213", 10.29, 4.0],
    ["RG-8X",   6.15, 3.5],
    ["RG-58",   4.95, 3.0],
];

// Selected parameters
bd          = band_data[band_index];
cd          = coax_data[coax_index];
band_name   = bd[0];
freq_mhz    = bd[1];
coax_name   = cd[0];
cable_od    = cd[1];
former_wall = cd[2];

// Former OD and turns (by coax type)
former_od   = coax_index == 0 ? bd[2] : coax_index == 1 ? bd[4] : bd[6];
turns       = coax_index == 0 ? bd[3] : coax_index == 1 ? bd[5] : bd[7];

// Derived
pitch           = cable_od + 0.5;
winding_len     = turns * pitch;
flange_thick    = 5.0;
flange_extend   = 10.0;
total_len       = winding_len + 2 * flange_thick;
flange_od       = former_od + 2 * flange_extend;
enc_clear       = 3.0;
enc_wall        = 3.5;
sr_len          = 12.0;
enc_id          = former_od + 2*former_wall + cable_od + 2*enc_clear;
enc_od          = enc_id + 2*enc_wall;
enc_total_len   = total_len + 2*sr_len + 2*enc_wall;

// ============================================================
// Visual assembly
// ============================================================
module show_former() {
    color("LightGray") {
        cylinder(d=former_od + 2*former_wall, h=total_len);
        translate([0,0,-0.01]) cylinder(d=flange_od, h=flange_thick+0.01);
        translate([0,0,total_len-flange_thick]) cylinder(d=flange_od, h=flange_thick+0.01);
    }
}

module show_winding() {
    color("Orange", 0.9) {
        for (i=[0:ceil(turns)-1]) {
            translate([0,0,flange_thick + i*pitch + pitch/2])
                rotate_extrude()
                    translate([(former_od + 2*former_wall)/2 - cable_od/4, 0, 0])
                        circle(d=cable_od);
        }
    }
}

module show_enclosure() {
    color("DarkGray", 0.4)
        difference() {
            translate([0,0,-sr_len-enc_wall])
                cylinder(d=enc_od, h=enc_total_len);
            translate([0,0,-sr_len-enc_wall-0.01])
                cylinder(d=enc_id, h=enc_total_len+0.02);
        }
}

// Render assembly
show_enclosure();
show_former();
show_winding();

// Cable stubs
color("Black") {
    translate([0,0,-sr_len-enc_wall-20])
        cylinder(d=cable_od, h=20);
    translate([0,0,total_len+sr_len+enc_wall])
        cylinder(d=cable_od, h=20);
}

// ============================================================
// Report parameters to console
// ============================================================
echo(str(""));
echo(str("===== TRAP ASSEMBLY: ", band_name, " @ ", freq_mhz, " MHz ====="));
echo(str("Coax type:      ", coax_name, " (OD=", cable_od, "mm)"));
echo(str("Former OD:      ", former_od, " mm"));
echo(str("Turns:          ", turns));
echo(str("Pitch:          ", pitch, " mm"));
echo(str("Winding length: ", winding_len, " mm"));
echo(str("Former length:  ", total_len, " mm"));
echo(str("Flange OD:      ", flange_od, " mm"));
echo(str("Enclosure OD:   ", enc_od, " mm"));
echo(str("Enclosure len:  ", enc_total_len, " mm"));
echo(str(""));
echo(str("PRINT CHECK — max dim = ", max(enc_od, enc_total_len), " mm"));
echo(str("Fits 200x200 bed: ", max(enc_od, enc_total_len) <= 195 ? "YES" : "SPLIT REQUIRED"));
echo(str(""));
echo(str("FABRICATION CHECKLIST:"));
echo(str("  [ ] Print former (trap_former.scad with od=", former_od, ")"));
echo(str("  [ ] Print enclosure halves (trap_enclosure.scad)"));
echo(str("  [ ] Cut ", coax_name, " coax to required length (see winding table)"));
echo(str("  [ ] Wind ", turns, " turns on ", former_od, "mm former"));
echo(str("  [ ] Seal ends with self-amalgamating tape"));
echo(str("  [ ] Install in enclosure, route cable through strain reliefs"));
echo(str("  [ ] Verify resonance with NanoVNA before weathersealing"));
echo(str("==========================================="));
