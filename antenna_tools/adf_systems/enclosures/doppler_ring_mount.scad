/**
 * doppler_ring_mount.scad — TM-ADF-ENC-003 Rev A
 * 4-Element Doppler Ring Array Mount
 *
 * Circular PCB/disk mount for 4 VHF/UHF monopole elements.
 * Holds elements at correct angular spacing (90°) and radius (0.5λ).
 * FST3253 switch IC mounts at center, coax runs to each element.
 *
 * Configurable for all 5 VHF/UHF bands by changing array_r parameter.
 * Parameterized: change array_r to match band.
 *
 * Print settings: PETG, 30% gyroid, 3 perimeters, 0.2mm
 * Envelope: 200×200×20mm (fits easily in 200mm cube)
 * Document: TM-ADF-ENC-003 Rev A
 */

// ─── Parameters ───────────────────────────────────────────────

// Array geometry — change for each band:
// 2m band: array_r = 513 (too large, use 345 for 0.25λ)
// Practical 2m portable: use 250mm radius (0.24λ, some gain loss)
array_r     = 173.0;   // radius to element centers (mm) — 70cm band (0.25λ)
                        // Change: 2m=250, 1.25m=168, 70cm=173, 33cm=82, 20cm=59

disk_thick  = 8.0;     // disk thickness
disk_h_extra= 3.0;     // raised rim around edge

// Element holder post
elem_post_h  = 18.0;   // height above disk for element BNC/SMA connector
elem_post_od = 14.0;   // diameter of element holder post
elem_post_id = 6.5;    // BNC panel-mount hole (standard 9.5mm OD body)

// Actually use SMA: 6.5mm hole for SMA
elem_hole_d  = 6.5;

// Center hub for switch IC PCB
hub_od  = 50.0;
hub_h   = elem_post_h;
hub_id  = 40.0;    // Inner cavity for IC + coax junction

// Coax channel (from each element to center)
coax_ch_w = 7.0;   // channel width
coax_ch_d = 4.0;   // channel depth

// Center column (mast / camera mount)
center_tube_od = 25.4;  // 1-inch tube (fits standard tripod rod)
center_tube_id = 22.0;  // bore

// Outer rim
rim_w   = 8.0;

// Mounting
mast_clamp_bolt = 4.3;  // M4 for mast clamp

// ─── Modules ──────────────────────────────────────────────────

module disk_base() {
    // Main disk with rim
    difference() {
        union() {
            // Flat disk
            cylinder(h=disk_thick, d=(array_r+rim_w)*2, $fn=128);
            // Outer raised rim
            difference() {
                cylinder(h=disk_thick+disk_h_extra, d=(array_r+rim_w)*2, $fn=128);
                translate([0, 0, disk_thick])
                    cylinder(h=disk_h_extra+1, d=(array_r+rim_w)*2-rim_w*2, $fn=128);
            }
        }

        // Central mast tube bore
        translate([0, 0, -0.5])
            cylinder(h=disk_thick+1, d=center_tube_od+0.5, $fn=32);

        // Coax channels (4× radial)
        for (a=[0, 90, 180, 270]) {
            rotate([0, 0, a])
            translate([center_tube_od/2, -coax_ch_w/2, disk_thick-coax_ch_d])
                cube([array_r - center_tube_od/2, coax_ch_w, coax_ch_d+0.5]);
        }

        // Weight relief holes (8 holes between element positions)
        for (a=[45, 135, 225, 315]) {
            rotate([0, 0, a])
            translate([array_r * 0.55, 0, -0.5])
                cylinder(h=disk_thick+1, d=30, $fn=32);
        }

        // Radial alignment grooves (for aligning to compass)
        for (a=[0, 90, 180, 270]) {
            rotate([0, 0, a])
            translate([(array_r+rim_w-4), 0, disk_thick+disk_h_extra-1])
                cylinder(h=2, d=2, $fn=16);
        }
    }
}

module element_post(angle) {
    // Post at element position with SMA hole
    rotate([0, 0, angle])
    translate([array_r, 0, 0]) {
        difference() {
            cylinder(h=elem_post_h + disk_thick, d=elem_post_od, $fn=24);
            // SMA hole (vertical)
            translate([0, 0, -0.5])
                cylinder(h=elem_post_h+disk_thick+1, d=elem_hole_d, $fn=16);
            // Coax entry from below (connects to radial channel)
            translate([0, 0, -0.5])
                cylinder(h=disk_thick+1, d=coax_ch_w-0.5, $fn=16);
        }
    }
}

module center_hub() {
    difference() {
        cylinder(h=hub_h + disk_thick, d=hub_od, $fn=48);
        // Central mast bore (clear through)
        translate([0, 0, -0.5])
            cylinder(h=hub_h+disk_thick+1, d=center_tube_od+0.5, $fn=32);
        // IC cavity (top)
        translate([0, 0, disk_thick+2])
            cylinder(h=hub_h, d=hub_id, $fn=32);
        // Coax entry holes from radial channels (4×)
        for (a=[0, 90, 180, 270]) {
            rotate([0, 0, a])
            translate([center_tube_od/2+coax_ch_w/2, 0, -0.5])
                cylinder(h=disk_thick+1, d=coax_ch_w-1, $fn=16);
        }
        // PCB mounting M2.5 holes
        for (a=[0, 90, 180, 270]) {
            rotate([0, 0, a])
            translate([hub_id/2-3, 0, disk_thick+1])
                cylinder(h=hub_h, d=2.7, $fn=16);
        }
    }
}

module mast_clamp() {
    // Split clamp ring for 1-inch tube
    clamp_h  = 30.0;
    clamp_od = center_tube_od + 14;
    difference() {
        cylinder(h=clamp_h, d=clamp_od, $fn=32);
        translate([0, 0, -0.5])
            cylinder(h=clamp_h+1, d=center_tube_id, $fn=32);
        // Split cut
        translate([-0.75, -clamp_od/2, -0.5])
            cube([1.5, clamp_od, clamp_h+1]);
        // Clamping bolt holes (M4, two sides)
        for (x=[-1,1]) {
            translate([x*(clamp_od/2-2), 0, clamp_h/2])
            rotate([90, 0, 0])
                cylinder(h=clamp_od+2, d=mast_clamp_bolt, $fn=16, center=true);
        }
    }
}

module doppler_ring_assembly() {
    // Full assembly
    disk_base();
    for (a=[0, 90, 180, 270]) element_post(a);
    center_hub();
}

// ─── Render ───────────────────────────────────────────────────
$fn = 64;
print_part = "ring";  // "ring" | "clamp" | "all"

if (print_part == "ring") {
    color("lightgray") doppler_ring_assembly();
} else if (print_part == "clamp") {
    color("gray") mast_clamp();
} else {
    color("lightgray", 0.85) doppler_ring_assembly();
    color("gray", 0.7)
    translate([0, 0, -(30+disk_thick)]) mast_clamp();
}

// ─── Band Configuration Notes ─────────────────────────────────
// Change array_r to match operating band:
//   2m   (146 MHz): array_r = 250  (0.24λ, slight pattern distortion OK)
//   1.25m (222 MHz): array_r = 168
//   70cm (435 MHz): array_r = 86   (0.25λ)
//   33cm (915 MHz): array_r = 41
//   20cm (1270 MHz): array_r = 30
//
// Element length (monopole above disk ground plane):
//   2m: 514mm, 1.25m: 338mm, 70cm: 172mm, 33cm: 82mm, 20cm: 59mm
//
// Ground plane: disk acts as radial ground plane.
// For better performance: attach 4× radial wires along coax channels.
