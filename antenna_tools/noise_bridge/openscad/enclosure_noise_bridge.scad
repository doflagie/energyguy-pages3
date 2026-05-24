// ============================================================
// NOISE BRIDGE WEATHERPROOF FIELD ENCLOSURE
// Fits CYD display, bridge PCB, noise gen, controls
// Weatherproof lid with O-ring seal for field use
//
// DIMENSIONS: 180 x 120 x 55 mm (fits 200x200 bed)
//
// Print settings:
//   Material: ASA (UV, temperature, chemical resistant)
//   Infill:   35% gyroid
//   Walls:    4 perimeters (weatherproofing)
//   Layer:    0.2mm
//   Support:  minimal, only for lid overhang
//
// Hardware:
//   6x M3 x 10mm stainless screws (lid)
//   1x M4 x 20mm stainless screw (carry handle)
//   1x 80 x 3mm O-ring (lid seal)
//   4x M3 x 6mm screws (CYD mounting)
//   4x rubber feet (3M SJ5012 or similar)
// ============================================================

/* [Box Dimensions] */
box_w    = 180.0;   // width (connector axis)
box_d    = 120.0;   // depth (front to back)
box_h    = 55.0;    // height (body, excluding lid)
wall     = 3.5;     // wall thickness
lid_h    = 15.0;    // lid height (overlap)
lid_wall = 3.5;

/* [CYD Display Cutout (ILI9341 2.8")] */
cyd_w    = 76.0;    // active area + bezel width
cyd_h    = 57.0;    // active area + bezel height
cyd_x    = box_w/2; // centered
cyd_y    = box_h - 5; // near top of lid face
// CYD viewing window (aperture only, CYD mounts inside)
cyd_win_w = 65.0;
cyd_win_h = 48.0;

/* [Front Panel Connectors] */
// SO-239 antenna jack
ant_x   = 20.0;   // from left edge
ant_y   = box_h/2;

// BNC noise in
noise_x = 55.0;
noise_y = box_h/2;

// BNC detector out
det_x   = 90.0;
det_y   = box_h/2;

// 3.5mm headphone
hp_x    = 130.0;
hp_y    = box_h/2;

/* [Control Panel (lid top)] */
// R pot (10-turn, 10mm shaft)
r_pot_x = 30.0;
r_pot_y = box_d/2 - 15;

// X pot (10-turn, 10mm shaft)
x_pot_x = 80.0;
x_pot_y = box_d/2 - 15;

// X direction switch (DPDT, 3mm hole for lever)
xdir_x  = 130.0;
xdir_y  = box_d/2 - 15;

// Power switch
pwr_x   = 155.0;
pwr_y   = 25.0;

// Noise source switch
noise_sw_x = 155.0;
noise_sw_y = 55.0;

/* [Battery Compartment] */
// 18V: 2x 9V batteries in series (PP3 size: 48.5 x 26.5 x 17.5mm each)
// Side compartment accessible from bottom flap
batt_w  = 55.0;    // 2 batteries side by side
batt_d  = 28.0;
batt_h  = 20.0;

/* [Mounting] */
boss_d  = 8.0;     // PCB mounting boss diameter
boss_h  = 5.0;     // boss height above floor
screw_d = 3.0;     // M3 screw (boss tap)
lid_bolt_d = 3.4;  // M3 clearance

$fn = 64;

// ============================================================
// O-ring groove
// ============================================================
module oring_groove(w, d, t=2.0, depth=1.5) {
    translate([0, 0, -0.01])
    difference() {
        cube([w + t, d + t, depth + 0.01], center=true);
        cube([w - t, d - t, depth + 0.02], center=true);
    }
}

// ============================================================
// PCB mounting boss
// ============================================================
module pcb_boss(x, y) {
    translate([x, y, 0]) {
        cylinder(d=boss_d, h=boss_h + wall);
        translate([0, 0, boss_h + wall - 0.01])
            cylinder(d=screw_d, h=15);  // tap hole guide
    }
}

// ============================================================
// Panel cutout helper
// ============================================================
module round_hole(d, depth=wall*3) {
    cylinder(d=d, h=depth, center=true);
}

module rect_hole(w, h, r=1, depth=wall*3) {
    hull() {
        for (x=[-w/2+r, w/2-r]) for (y=[-h/2+r, h/2+r-2*r])
            translate([x, y, 0]) cylinder(r=r, h=depth, center=true);
    }
}

// ============================================================
// MAIN BOX BODY
// ============================================================
module box_body() {
    difference() {
        // Outer shell
        union() {
            cube([box_w, box_d, box_h]);
            // Lid engagement rail (inner lip for O-ring seating)
            translate([wall, wall, box_h - 1])
                cube([box_w - 2*wall, box_d - 2*wall, 2]);
        }

        // Interior cavity
        translate([wall, wall, wall])
            cube([box_w - 2*wall, box_d - 2*wall, box_h]);

        // Battery compartment (rear cutout, accessible from bottom)
        translate([box_w - batt_w - wall*2, box_d - batt_d - wall*2, -0.01])
            cube([batt_w, batt_d, batt_h + wall + 1]);

        // Front panel: SO-239 antenna
        translate([ant_x, -0.01, ant_y])
            rotate([-90, 0, 0]) round_hole(21.0);  // SO-239

        // Front panel: BNC noise in
        translate([noise_x, -0.01, noise_y])
            rotate([-90, 0, 0]) round_hole(12.0);  // BNC

        // Front panel: BNC detector out
        translate([det_x, -0.01, det_y])
            rotate([-90, 0, 0]) round_hole(12.0);

        // Front panel: 3.5mm headphone
        translate([hp_x, -0.01, hp_y])
            rotate([-90, 0, 0]) round_hole(7.0);

        // Lid bolt holes (perimeter, 6x M3)
        for (x=[20, box_w/2, box_w-20]) {
            translate([x, box_d - wall/2, box_h + 0.5])
                cylinder(d=lid_bolt_d, h=10, center=true);
            translate([x, wall/2, box_h + 0.5])
                cylinder(d=lid_bolt_d, h=10, center=true);
        }

        // Label bosses on front panel
        translate([box_w/2, -0.01, 12])
            rotate([-90, 0, 0])
                cube([120, 10, wall*0.5], center=true);

        // Drain/vent hole (bottom rear, tiny, 2mm)
        translate([wall + 5, box_d - wall - 5, -0.01])
            cylinder(d=2, h=wall + 2);
    }

    // PCB mounting bosses (4 corners of main PCB area)
    translate([0, 0, wall]) {
        pcb_boss(15, 15);
        pcb_boss(box_w - 15, 15);
        pcb_boss(15, box_d - 15);
        pcb_boss(box_w - 15, box_d - 15);
        // Additional center bosses for bridge PCB
        pcb_boss(box_w/2 - 25, box_d/2);
        pcb_boss(box_w/2 + 25, box_d/2);
    }

    // Battery retainer clips
    translate([box_w - batt_w - wall*2 + 5, box_d - batt_d - wall*2, batt_h])
        cube([batt_w - 10, 3, 5]);
}

// ============================================================
// LID (with CYD window and control holes)
// ============================================================
module box_lid() {
    translate([0, 0, box_h + 5])  // offset for print preview
    difference() {
        union() {
            // Main lid panel
            cube([box_w, box_d, wall]);
            // Engagement skirt (drops inside box)
            translate([wall/2, wall/2, -lid_h + wall])
                difference() {
                    cube([box_w - wall, box_d - wall, lid_h]);
                    translate([wall, wall, -0.01])
                        cube([box_w - wall*3, box_d - wall*3, lid_h + 0.02]);
                }
        }

        // CYD display window (centered on lid)
        translate([cyd_x, box_d*0.35, wall/2])
            rect_hole(cyd_win_w, cyd_win_h, 2);

        // R pot shaft hole
        translate([r_pot_x, r_pot_y, 0])
            cylinder(d=10.5, h=wall*3, center=true);
        // R pot lock flat
        translate([r_pot_x + 6, r_pot_y, 0])
            cube([3, 8, wall*3], center=true);

        // X pot shaft hole
        translate([x_pot_x, x_pot_y, 0])
            cylinder(d=10.5, h=wall*3, center=true);
        translate([x_pot_x + 6, x_pot_y, 0])
            cube([3, 8, wall*3], center=true);

        // X direction switch (mini toggle: 6mm hole)
        translate([xdir_x, xdir_y, 0])
            cylinder(d=6.5, h=wall*3, center=true);

        // Power switch (15mm rectangular mini rocker)
        translate([pwr_x, pwr_y, 0])
            rect_hole(15, 10, 1);

        // Noise switch
        translate([noise_sw_x, noise_sw_y, 0])
            rect_hole(15, 10, 1);

        // O-ring groove (on underside of lid, mates with box rail)
        translate([box_w/2, box_d/2, 0])
            oring_groove(box_w - wall*3, box_d - wall*3);

        // Lid bolt holes (matching box)
        for (x=[20, box_w/2, box_w-20]) {
            translate([x, box_d - wall/2, 0])
                cylinder(d=lid_bolt_d, h=wall*3, center=true);
            translate([x, wall/2, 0])
                cylinder(d=lid_bolt_d, h=wall*3, center=true);
        }

        // CYD mounting holes (4x M3)
        cyd_mount_offset_x = 3.5; // from window edge
        cyd_mount_offset_y = 3.5;
        for (dx=[-1,1]) for (dy=[-1,1]) {
            translate([cyd_x + dx*(cyd_win_w/2 + cyd_mount_offset_x),
                       box_d*0.35 + dy*(cyd_win_h/2 + cyd_mount_offset_y), 0])
                cylinder(d=3.2, h=wall*3, center=true);
        }
    }
}

// ============================================================
// Battery door (slides into rear slot)
// ============================================================
module battery_door() {
    translate([box_w + 10, 0, 0])
    difference() {
        cube([batt_w + wall*2, batt_d + wall*2, wall + 2]);
        translate([wall, wall, -0.01])
            cube([batt_w, batt_d, wall + 3]);
        // Thumb notch
        translate([batt_w/2 + wall, batt_d + wall*1.5, wall/2])
            cylinder(d=10, h=wall*2, center=true);
    }
}

// ============================================================
// Rubber foot recess (4x, bottom corners)
// ============================================================
module foot_recess() {
    translate([0, 0, -0.01])
        cylinder(d=12, h=3);
}

// ============================================================
// Carry strap lug (top rear)
// ============================================================
module strap_lug() {
    translate([box_w/2, box_d - wall, box_h - 10])
    difference() {
        hull() {
            cube([20, wall*2, 20], center=true);
            translate([0, -5, 0]) cube([28, 1, 24], center=true);
        }
        // Strap slot
        cube([14, wall*4, 10], center=true);
        // M4 bolt hole
        translate([0, 0, -5]) cylinder(d=4.4, h=25, center=true);
    }
}

// ============================================================
// Render all parts
// ============================================================
box_body();
box_lid();
battery_door();

// Foot recesses (on bottom)
for (x=[12, box_w-12]) for (y=[12, box_d-12]) {
    translate([x, y, 0]) foot_recess();
}

strap_lug();

// ============================================================
// Dimension check
// ============================================================
echo("=== NOISE BRIDGE ENCLOSURE ===");
echo(str("Box: ", box_w, " x ", box_d, " x ", box_h, " mm"));
echo(str("Lid: ", box_w, " x ", box_d, " x ", wall, " mm + ", lid_h, "mm skirt"));
echo(str("Fits 200mm bed: ", max(box_w, box_d) <= 195 ? "YES" : "NO (split required)"));
echo("O-ring: 80x3mm standard (inner radius ~77mm). Adjust oring_groove if needed.");
echo("Connectors: Front face. Controls: Lid top. CYD: Lid center.");
echo("Battery: Rear compartment, 2x 9V for 18V supply.");
