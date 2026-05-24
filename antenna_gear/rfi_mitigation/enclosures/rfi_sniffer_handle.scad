/**
 * rfi_sniffer_handle.scad — TM-RFI-ENC-004 Rev A
 * RFI Near-Field Sniffer Probe Handle
 *
 * Two configurations sharing same handle body:
 *   Config A: H-field shielded loop (100mm loop, RG-178 coax)
 *   Config B: E-field rod probe (100mm rod, JFET preamp in handle)
 *
 * Features:
 *   - Ergonomic pistol-grip handle, 130mm long
 *   - Probe tip snaps into front socket (interchangeable tips)
 *   - Preamp PCB cavity: 30×20×12 mm (optional, for E-field active version)
 *   - Cable exit: rear (SMA(F) connector or strain-relief for RG-178)
 *   - Battery cavity: CR2032 × 1 (for preamp power if no bias-T)
 *   - Loop tip socket: holds 100mm loop securely
 *   - Rod tip socket: holds 100mm rod securely
 *
 * Envelope: 150×45×35 mm
 */

// ── Parameters ─────────────────────────────────────────────────────────────
// Handle body
handle_l  = 130.0;  // length
handle_w  = 35.0;   // width
handle_h  = 42.0;   // height (grip height)
wall      = 2.5;
r = 8.0;
$fn = 32;

// Probe tip socket (front)
tip_bore_d  = 10.0;   // bore for SMA barrel or coax
tip_bore_l  = 18.0;   // depth of socket

// PCB cavity (for preamp)
pcb_w   = 32.0;
pcb_d   = 22.0;
pcb_h   = 14.0;

// Battery cavity (CR2032: 20mm dia × 3.2mm)
batt_d  = 21.0;
batt_h  = 4.5;

// Cable exit (rear SMA: 6.5mm hole, or strain relief: 9mm)
rear_exit_d = 6.5;   // SMA(F) or strain relief

// Set screw (probe tip retention, M3)
sscrew_d = 3.3;

// Mounting hole (for lanyard or bench mount)
lanyard_d = 4.0;

// ── Body ─────────────────────────────────────────────────────────────────────
module handle_body() {
    difference() {
        // Outer ergonomic shape: rounded cuboid with finger indentations
        hull()
            for (x=[r, handle_l-r]) for (y=[r, handle_w-r]) for (z=[r, handle_h-r])
                translate([x, y, z]) sphere(r=r, $fn=16);

        // Interior cavity (preamp + battery space)
        translate([handle_l/2 - pcb_w/2, wall, handle_h - pcb_h - wall])
            cube([pcb_w, pcb_d, pcb_h + 1]);

        // Battery cavity (side access)
        translate([handle_l/2 - batt_d/2, handle_w/2 - batt_d/2, handle_h - batt_h - wall])
            cylinder(h=batt_h+wall+1, d=batt_d, $fn=24);

        // Probe tip socket (front face)
        translate([-0.5, handle_w/2, handle_h/2])
        rotate([0, 90, 0])
            cylinder(h=tip_bore_l+1, d=tip_bore_d, $fn=16);

        // Cable exit (rear face, SMA)
        translate([handle_l+0.5, handle_w/2, handle_h/2])
        rotate([0, -90, 0])
            cylinder(h=wall+1, d=rear_exit_d, $fn=16);

        // Set screw for probe tip retention (top, near front)
        translate([tip_bore_l/2, handle_w/2, handle_h + 0.5])
        rotate([180, 0, 0])
            cylinder(h=wall+1, d=sscrew_d, $fn=16);

        // Lanyard hole (rear bottom)
        translate([handle_l - 12, -0.5, handle_h/3])
        rotate([-90, 0, 0])
            cylinder(h=handle_w+1, d=lanyard_d, $fn=16);

        // Finger groove (makes grip ergonomic) — slight indentation on bottom
        translate([handle_l/2, -0.5, -2])
            scale([1, 0.5, 1])
            cylinder(h=handle_h/2, r=handle_w*0.4, $fn=32);

        // Top access slot (for accessing PCB/battery — covered by snap lid)
        translate([handle_l/2 - pcb_w/2 + 2, handle_w/2 - pcb_d/2 + 2,
                   handle_h - wall - 0.5])
            cube([pcb_w-4, pcb_d-4, wall+1]);
    }
}

// ── Probe tip: H-field loop socket ────────────────────────────────────────
// Accepts 100mm diameter shielded loop (made from RG-178 coax)
module loop_tip_socket() {
    // SMA barrel fits in tip_bore; loop body is larger
    // This adaptor: 10mm bore back, 15mm collar front
    difference() {
        cylinder(h=22, d=16, $fn=24);
        // SMA barrel bore
        translate([0,0,-0.5]) cylinder(h=22+1, d=tip_bore_d, $fn=16);
        // Loop plane slot (narrow slot for loop to exit sideways)
        translate([-1, -0.5, 12]) cube([2, 16, 12]);  // slot for loop
    }
}

// ── Probe tip: E-field rod socket ────────────────────────────────────────
// 100mm rod slides in
module rod_tip_socket() {
    rod_d = 2.5;   // 2mm rod + clearance
    difference() {
        cylinder(h=22, d=10, $fn=24);
        translate([0,0,-0.5]) cylinder(h=22+1, d=rod_d, $fn=16);
    }
}

// ── Render ─────────────────────────────────────────────────────────────────
print_part = "handle";  // "handle" | "loop_tip" | "rod_tip"

if (print_part == "handle") {
    color("darkgreen", 0.9) handle_body();
} else if (print_part == "loop_tip") {
    color("olive", 0.9) loop_tip_socket();
} else {
    color("darkolivegreen", 0.9) rod_tip_socket();
}

// ── Assembly Notes ──────────────────────────────────────────────────────────
// H-FIELD LOOP ASSEMBLY:
//   1. Print handle + loop_tip_socket.
//   2. Form 100mm loop from RG-178 miniature coax (34cm length).
//   3. Solder SMA(M) at one end; thread loop body through slot in loop_tip_socket.
//   4. Secure loop to socket with epoxy or cable tie.
//   5. Insert SMA barrel into handle tip bore; secure with M3 set screw.
//   6. Install SPF5189Z or MAR-6+ preamp PCB in PCB cavity.
//   7. Connect coax from loop SMA → preamp → output SMA at rear.
//   8. Power: CR2032 in battery cavity, or bias-T from SDR (remove battery).
//
// E-FIELD ROD ASSEMBLY:
//   1. Print handle + rod_tip_socket.
//   2. Press-fit 2mm stainless rod into rod_tip_socket bore.
//   3. Connect rod wire to J310 JFET gate (in PCB cavity).
//   4. Install J310 source follower PCB in PCB cavity.
//   5. Route output to rear SMA.
//   6. ALWAYS add 20 dB SMA attenuator at SDR input when using E-field probe.
