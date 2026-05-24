// ============================================================
// 1kW OIL-COOLED DUMMY LOAD ENCLOSURE
// Welded aluminum tank design
// This file generates: lid assembly + connector mount plate
//
// NOTE: Tank body is welded aluminum (6061-T6, 3mm wall)
//       Dimensions: 150 x 150 x 320 mm interior
//       Alternatively: stainless hotel food pan with custom lid
//
// This file produces:
//   1. Top lid plate (printed ABS/PETG as template for Al machining,
//      OR print directly in ASA for prototype use)
//   2. Connector insert (bottom of tank)
//   3. Resistor bus ring jig (alignment tool for winding buses)
//   4. Resistor bank spacer (keeps banks at correct height)
//
// Print: ASA, 3 perimeters, 35% infill
// Post: Lid should be machined from 6mm 6061-T6 for production
// ============================================================

/* [Tank Dimensions (interior)] */
tank_w = 150.0;
tank_d = 150.0;
tank_h = 320.0;

/* [Lid Parameters] */
lid_t      = 8.0;    // lid thickness
flange_w   = 10.0;   // flange width (overhangs tank walls)
bolt_d     = 4.4;    // M4 clearance
bolt_rows  = 4;      // bolts per side
gasket_w   = 5.0;    // O-ring groove width
gasket_h   = 2.5;    // O-ring groove depth

/* [Fill/Drain/Probe Ports] */
fill_d     = 13.0;   // 1/2" NPT fill port
drain_d    = 9.7;    // 3/8" NPT drain (bottom)
probe_d    = 7.0;    // 1/4" NPT temp probe

/* [Electrical Ports] */
// N-type connector
n_hole_d   = 21.0;
// SMA ports (coupler)
sma_d      = 6.3;
// Ground stud
gnd_d      = 6.5;   // 1/4"-20 ground stud

/* [Resistor Bank Geometry] */
n_banks    = 5;
r_per_bank = 4;
bank_pitch = 55.0;  // mm between banks (from base)
ring_od    = 90.0;  // outer ring OD
ring_id_inner = 20.0;  // inner ring ID (around center stud)

$fn = 64;

// ============================================================
// Lid with all ports
// ============================================================
module tank_lid() {
    ext_w = tank_w + 2*flange_w;
    ext_d = tank_d + 2*flange_w;

    difference() {
        cube([ext_w, ext_d, lid_t], center=true);

        // Fill port
        translate([tank_w/4, 0, 0])
            cylinder(d=fill_d, h=lid_t*2, center=true);

        // Temp probe ports (2x: oil and air)
        translate([-tank_w/4, tank_d/4, 0])
            cylinder(d=probe_d, h=lid_t*2, center=true);
        translate([-tank_w/4, -tank_d/4, 0])
            cylinder(d=probe_d, h=lid_t*2, center=true);

        // Pressure relief
        translate([0, tank_d/4, 0])
            cylinder(d=8.6, h=lid_t*2, center=true);

        // Center stud pass-through (1/4"-20 brass stud, clearance)
        cylinder(d=8.0, h=lid_t*2, center=true);

        // O-ring groove
        translate([0, 0, lid_t/2 - gasket_h/2 - 0.5])
        difference() {
            cube([tank_w + gasket_w, tank_d + gasket_w, gasket_h + 0.01],
                 center=true);
            cube([tank_w - gasket_w, tank_d - gasket_w, gasket_h + 0.02],
                 center=true);
        }

        // Bolt holes (perimeter, M4)
        for (side = [0:3]) {
            for (i = [0:bolt_rows-1]) {
                a = side * 90;
                x_base = side == 0 || side == 2 ? 0 :
                         side == 1 ? tank_w/2 + flange_w/2 : -(tank_w/2 + flange_w/2);
                y_base = side == 1 || side == 3 ? 0 :
                         side == 0 ? tank_d/2 + flange_w/2 : -(tank_d/2 + flange_w/2);
                // Place bolts evenly along each side
                x = side == 0 || side == 2 ?
                    (i - (bolt_rows-1)/2) * (tank_w/bolt_rows) : x_base;
                y = side == 1 || side == 3 ?
                    (i - (bolt_rows-1)/2) * (tank_d/bolt_rows) : y_base;
                translate([x == 0 ? x_base : x,
                           y == 0 ? y_base : y, 0])
                    cylinder(d=bolt_d, h=lid_t*2, center=true);
            }
        }
    }

    // Port bosses
    translate([tank_w/4, 0, lid_t/2])
        cylinder(d=fill_d + 10, h=8);
    translate([-tank_w/4, tank_d/4, lid_t/2])
        cylinder(d=probe_d + 8, h=8);
    translate([0, tank_d/4, lid_t/2])
        cylinder(d=relief_d + 8, h=6);
}

relief_d = 8.6;

// ============================================================
// Bottom connector plate (inside tank, mounts N-connector)
// ============================================================
module connector_plate() {
    // Mounts to inside bottom of tank
    // N-connector centered, coax stud protrudes upward
    difference() {
        cube([tank_w - 2, tank_d - 2, 8], center=true);
        // N-connector hole
        cylinder(d=n_hole_d, h=10, center=true);
        // N bolt holes
        for (a=[45,135,225,315]) {
            translate([12.7*cos(a), 12.7*sin(a), 0])
                cylinder(d=2.9, h=10, center=true);
        }
        // SMA ports (coupler, 2x side mount)
        translate([tank_w/2 - 1, 20, 0])
            rotate([0, 90, 0]) cylinder(d=sma_d, h=10, center=true);
        translate([tank_w/2 - 1, -20, 0])
            rotate([0, 90, 0]) cylinder(d=sma_d, h=10, center=true);
        // Ground stud
        translate([tank_w/4, tank_d/4, 0])
            cylinder(d=gnd_d, h=10, center=true);
    }
}

// ============================================================
// Resistor bus ring (alignment jig + permanent part)
// One per bank, 5 banks total
// ============================================================
module bus_ring_outer() {
    // Outer ground ring — resistor cold ends solder to this
    difference() {
        cylinder(d=ring_od, h=3);
        translate([0, 0, -0.01])
            cylinder(d=ring_od - 8, h=3.02);
        // 4x notches for resistor wire entry
        for (a=[0, 90, 180, 270]) {
            rotate([0, 0, a])
                translate([ring_od/2 - 2, 0, 0])
                    cube([6, 3, 4], center=true);
        }
    }
}

module bus_ring_inner() {
    // Inner hot ring — resistor hot ends and center stud
    difference() {
        cylinder(d=ring_id_inner + 8, h=3);
        translate([0, 0, -0.01])
            cylinder(d=ring_id_inner, h=3.02);
        for (a=[0, 90, 180, 270]) {
            rotate([0, 0, a])
                translate([(ring_id_inner + 4), 0, 0])
                    cube([4, 3, 4], center=true);
        }
    }
}

// ============================================================
// Center stud insulating bushing (per lid)
// Print from PTFE or Nylon; nylon is oil-compatible
// ============================================================
module stud_bushing() {
    difference() {
        cylinder(d=20, h=lid_t + 2);
        translate([0, 0, -0.01]) cylinder(d=7.5, h=lid_t + 3); // 5/16-18 stud
    }
}

// ============================================================
// Render all parts
// ============================================================

// Lid
translate([0, 0, 0])
    tank_lid();

// Connector plate
translate([(tank_w + 2*flange_w) + 20, 0, 4])
    connector_plate();

// Bus rings (show one set; print 5 of each)
translate([0, (tank_d + 2*flange_w) + 20, 0]) {
    bus_ring_outer();
    translate([ring_od + 15, 0, 0]) bus_ring_inner();
    translate([ring_od*2 + 30, 0, 0]) stud_bushing();
}

echo("=== 1kW OIL DUMMY LOAD ===");
echo("Tank body: weld from 3mm 6061-T6 aluminum plate");
echo(str("Interior: ", tank_w, " x ", tank_d, " x ", tank_h, " mm"));
echo("Oil volume: ~4.5 liters naphthenic transformer oil");
echo("Print: lid, connector plate, bus rings in ASA");
echo("Production lid: machine from 6mm 6061-T6");
echo("Bus rings: cut from copper sheet or print as winding jig");
echo("Resistors: 20x Ohmite RH-50E1K (1000R 50W)");
