// ============================================================
// NOISE BRIDGE FRONT PANEL LABEL PLATE
// Engraved labels for connector and control positions
// Print in contrasting color, glue to front face of enclosure
//
// Also includes: knob scales for R and X dials
//
// Print: 0.1mm layer height, no infill, 2 walls
// Material: Any rigid filament (PLA acceptable for label plate)
// ============================================================

/* [Panel Dimensions (matches enclosure front face)] */
panel_w  = 180.0;
panel_h  = 55.0;
panel_t  = 1.5;   // thin plate, glue-on

/* [Connector positions (match enclosure_noise_bridge.scad)] */
ant_x    = 20.0;
noise_x  = 55.0;
det_x    = 90.0;
hp_x     = 130.0;
connector_y = panel_h / 2;

/* [Knob scale parameters] */
scale_r_outer = 22.0;   // outer radius of scale ring
scale_r_inner = 17.0;   // inner radius (tick marks)
scale_marks   = 20;     // number of tick marks

$fn = 64;

// ============================================================
// Engraved text simulation (embossed rectangles for labels)
// In practice, use label maker or paint/fill the recesses
// ============================================================
module label_box(x, y, w, h) {
    translate([x - w/2, y - h/2, -0.01])
        cube([w, h, panel_t * 0.4]);
}

// ============================================================
// Connector cutout (clearance hole through label plate)
// ============================================================
module conn_hole(x, y, d) {
    translate([x, y, -0.01])
        cylinder(d=d + 1, h=panel_t + 0.02);
}

// ============================================================
// Front label plate
// ============================================================
module front_panel() {
    difference() {
        cube([panel_w, panel_h, panel_t]);

        // Connector holes
        conn_hole(ant_x,   connector_y, 21.0);  // SO-239
        conn_hole(noise_x, connector_y, 12.0);  // BNC
        conn_hole(det_x,   connector_y, 12.0);  // BNC
        conn_hole(hp_x,    connector_y, 7.0);   // 3.5mm

        // Label areas (recessed, to be filled with paint or ink)
        label_box(ant_x,   connector_y - 18, 30, 5);  // "ANT"
        label_box(noise_x, connector_y - 18, 35, 5);  // "NOISE IN"
        label_box(det_x,   connector_y - 18, 35, 5);  // "DET/RX"
        label_box(hp_x,    connector_y - 18, 30, 5);  // "PHONES"

        label_box(ant_x,   connector_y + 18, 40, 5);  // "50 OHM"
        label_box(noise_x, connector_y + 18, 30, 5);  // "BNC"
        label_box(det_x,   connector_y + 18, 30, 5);  // "BNC"
        label_box(hp_x,    connector_y + 18, 35, 5);  // "3.5mm"

        // Product name label area
        label_box(panel_w/2, 8, 90, 6);   // "RF NOISE BRIDGE"
        label_box(panel_w/2, 48, 60, 5);  // "TM-NB-001"

        // "R" and "X" connector indicator ticks
        for (x=[ant_x, noise_x, det_x, hp_x]) {
            translate([x, panel_h/2 - 1, panel_t - 0.4])
                cube([0.5, 2, 0.5]);
        }
    }
}

// ============================================================
// R dial scale (print separately, glue to R pot knob)
// ============================================================
module r_dial_scale() {
    translate([0, panel_h + 30, 0])
    difference() {
        cylinder(r=scale_r_outer, h=panel_t);
        cylinder(r=scale_r_inner - 3, h=panel_t * 3, center=true);
        // Center shaft hole
        cylinder(d=6.5, h=panel_t * 3, center=true);
        // Tick marks (0-200 ohm, 20 divisions = 10 ohm each)
        for (i=[0:scale_marks-1]) {
            a = -270 * i / scale_marks; // 270 degree sweep
            is_major = (i % 4 == 0);
            tick_w  = is_major ? 1.5 : 0.8;
            tick_l  = is_major ? 7.0 : 4.0;
            translate([0, 0, -0.01])
            rotate([0, 0, a - 135])  // start at 7 o'clock, end at 5 o'clock
                translate([scale_r_inner + tick_l/2 - 1, 0, 0])
                    cube([tick_l, tick_w, panel_t + 0.02], center=true);
        }
        // "0" and "200" marker grooves
        // (Labels added by hand or with label maker)
    }
}

// ============================================================
// X dial scale (bipolar: -200 to +200, center = 0)
// ============================================================
module x_dial_scale() {
    translate([50, panel_h + 30, 0])
    difference() {
        cylinder(r=scale_r_outer, h=panel_t);
        cylinder(r=scale_r_inner - 3, h=panel_t * 3, center=true);
        cylinder(d=6.5, h=panel_t * 3, center=true);
        // Center mark (0, purely resistive)
        translate([scale_r_inner + 2, 0, -0.01])
            cube([8, 2, panel_t + 0.02], center=true);
        // Positive (+X, capacitive) marks
        for (i=[1:10]) {
            a = 13.5 * i;  // 135 degrees for full range each side
            translate([0,0,-0.01])
            rotate([0,0, a])
                translate([scale_r_inner + 4, 0, 0])
                    cube([(i % 2 == 0) ? 7 : 4, 0.8, panel_t+0.02], center=true);
        }
        // Negative (-X, inductive) marks
        for (i=[1:10]) {
            a = -13.5 * i;
            translate([0,0,-0.01])
            rotate([0,0, a])
                translate([scale_r_inner + 4, 0, 0])
                    cube([(i % 2 == 0) ? 7 : 4, 0.8, panel_t+0.02], center=true);
        }
    }
}

// ============================================================
// Render
// ============================================================
front_panel();
r_dial_scale();
x_dial_scale();

echo("=== FRONT PANEL ===");
echo(str("Panel: ", panel_w, " x ", panel_h, " mm"));
echo("Print in contrasting color. Fill label recesses with paint.");
echo("Dial scales: R (0-200 ohm), X (-200 to +200 ohm)");
echo("Glue dial scales to potentiometer knobs.");
