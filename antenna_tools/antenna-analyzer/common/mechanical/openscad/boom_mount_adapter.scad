// INTEGRATED ANTENNA ANALYZER - BOOM MOUNT ADAPTER
// Universal mounting adapter for antenna boom or tower attachment
// Accommodates 1" round or 1" square aluminum boom

// Boom dimensions (adaptive)
boom_diameter_round = 25.4;    // 1" round boom in mm
boom_width_square = 25.4;      // 1" square boom, 25.4mm sides

// Adapter dimensions
adapter_thickness = 4;         // 4mm thickness for aluminum
clamp_width = 80;              // Clamp span (two halves)
clamp_height = 30;             // Height to accommodate boom + fasteners

// U-bolt specifications (typical boom clamp)
ubolt_diameter = 8;            // 8mm steel U-bolt
fastener_spacing = 60;         // Center-to-center of fastener holes

// Attachment to analyzer
analyzer_tripod_boss_diameter = 12;  // 1/4"-20 UNC threaded mount boss
analyzer_thread_size = 5;      // M5 equivalent (1/4"-20 UNC)

// Clamp jaw specifications
jaw_inner_radius = 14;         // Radius of curved jaw (accommodates 1" boom)
jaw_outer_radius = 20;         // Outer radius of jaw
jaw_thickness = 4;             // Thickness of clamping jaw

module boom_clamp_upper_jaw() {
  // Upper half of boom clamp (removable/hinged)
  // Curves to fit 1" round or square boom

  // Main clamp body
  difference() {
    // Outer envelope
    translate([0, 0, 0])
      cube([clamp_width, clamp_height, adapter_thickness]);

    // Curved inner surface (fits boom contour)
    translate([clamp_width/2, clamp_height/2, -2])
      cylinder(h = adapter_thickness + 4, r = jaw_inner_radius, $fn = 64);
  }

  // U-bolt fastener pads
  // Two mounting holes for U-bolt nuts

  ubolt_y_offset = 15;  // Distance from clamp center

  // Left fastener boss
  translate([clamp_width/2 - fastener_spacing/2, ubolt_y_offset, adapter_thickness])
    cylinder(h = 8, d = 12, $fn = 32);

  // Right fastener boss
  translate([clamp_width/2 + fastener_spacing/2, ubolt_y_offset, adapter_thickness])
    cylinder(h = 8, d = 12, $fn = 32);

  // Center vertical mounting post (for analyzer tripod mount)
  translate([clamp_width/2, clamp_height - 10, adapter_thickness])
    cube([30, 15, 15]);
}

module boom_clamp_lower_jaw() {
  // Lower half of boom clamp (stationary base)
  // Matches upper jaw, provides structural base

  difference() {
    // Outer envelope
    translate([0, 0, -adapter_thickness])
      cube([clamp_width, clamp_height, adapter_thickness]);

    // Curved inner surface (matches upper jaw)
    translate([clamp_width/2, clamp_height/2, -adapter_thickness - 2])
      cylinder(h = adapter_thickness + 4, r = jaw_inner_radius, $fn = 64);
  }

  // U-bolt fastener pads (match upper jaw)
  ubolt_y_offset = 15;

  translate([clamp_width/2 - fastener_spacing/2, ubolt_y_offset, -adapter_thickness])
    cylinder(h = 8, d = 12, $fn = 32);

  translate([clamp_width/2 + fastener_spacing/2, ubolt_y_offset, -adapter_thickness])
    cylinder(h = 8, d = 12, $fn = 32);
}

module center_mounting_post() {
  // Central post that connects to analyzer tripod mount
  // 1/4"-20 UNC threaded hole for analyzer boss

  post_width = 35;
  post_height = 25;
  post_z_offset = adapter_thickness;  // Mounts on top of lower jaw

  difference() {
    // Main post
    translate([clamp_width/2 - post_width/2, clamp_height - 15, post_z_offset])
      cube([post_width, 20, post_height]);

    // Threaded hole for analyzer mount
    translate([clamp_width/2, clamp_height - 5, post_z_offset + 2])
      cylinder(h = post_height, d = analyzer_thread_size + 0.5, $fn = 32);
  }
}

module fastener_holes() {
  // Clearance holes for U-bolt fasteners (8mm diameter)

  ubolt_y = 15;
  hole_depth = 12;

  // Left fastener hole
  translate([clamp_width/2 - fastener_spacing/2, ubolt_y, -2])
    cylinder(h = hole_depth + 4, d = ubolt_diameter, $fn = 32);

  // Right fastener hole
  translate([clamp_width/2 + fastener_spacing/2, ubolt_y, -2])
    cylinder(h = hole_depth + 4, d = ubolt_diameter, $fn = 32);
}

module cable_routing_clips() {
  // Clips to route analyzer power/signal cables along boom
  // Prevents cable movement and stress on connectors

  clip_spacing = 150;   // Spacing along boom

  // Bottom-mounted clips (every ~150mm along boom)
  for (i = [0:2]) {
    clip_x = (i * clip_spacing) + 20;
    clip_y = 8;
    clip_z = -adapter_thickness - 3;

    translate([clip_x, clip_y, clip_z])
      cube([15, 5, 3]);  // Thin clip protruding downward
  }
}

module labeling_and_markings() {
  // Surface features for clarity (engraved or raised)
  // Indicates clamp orientation and analyzer position

  text_depth = 0.5;

  // "UP" arrow indicating correct mounting orientation
  translate([clamp_width/2 - 8, 5, adapter_thickness])
    cube([16, 8, text_depth]);

  // Center line marking (helps with boom centering)
  translate([clamp_width/2 - 1, 0, adapter_thickness])
    cube([2, clamp_height, text_depth]);
}

module side_cable_guides() {
  // Side guides to keep analyzer cables neat
  // Prevents RF interference from cable radiation

  guide_height = 15;
  guide_width = 8;

  // Left side guide
  translate([2, clamp_height - 20, adapter_thickness])
    cube([guide_width, 25, guide_height]);

  // Right side guide
  translate([clamp_width - 2 - guide_width, clamp_height - 20, adapter_thickness])
    cube([guide_width, 25, guide_height]);
}

module assembly() {
  // Complete boom mount assembly

  // Lower jaw (stationary base)
  boom_clamp_lower_jaw();

  // Upper jaw (removable)
  translate([0, 0, adapter_thickness * 2])  // Offset for visualization
    boom_clamp_upper_jaw();

  // Center mounting post
  center_mounting_post();

  // Cable guides
  side_cable_guides();

  // Labeling
  labeling_and_markings();

  // Cable routing clips
  cable_routing_clips();
}

// RENDER ASSEMBLY
difference() {
  assembly();
  fastener_holes();
}

// Show boom position for reference (visualization only)
%translate([clamp_width/2, clamp_height/2, -5])
  cylinder(h = 40, d = boom_diameter_round, $fn = 32);  // 1" round boom visualization

// ASSEMBLY INSTRUCTIONS FOR USER:
// 1. Slide analyzer base adapter around 1" boom
// 2. Upper and lower jaw halves sandwich boom
// 3. Insert U-bolt (8mm) through fastener holes
// 4. Tighten U-bolt nuts evenly (alternating sides)
// 5. Do NOT over-tighten (avoid distorting boom)
// 6. Connect analyzer to center post via 1/4"-20 screw
// 7. Route DUT/REF cables along side guides
// 8. Secure cables with small zip ties through clips
// 9. Analyzer hangs vertically (display faces down toward operator)
// 10. Orient DUT/REF ports: DUT toward antenna, REF toward reference

// ALTERNATIVE MOUNTING OPTIONS:
// A) Direct antenna attachment: mount analyzer at antenna base
// B) Tripod mounting: use standard photography tripod (1/4"-20 female socket)
// C) Boom mounting: this adapter (best for mobile antenna measurements)

// NOTES FOR MANUFACTURING:
// 1. Print in strong PETG or nylon (load-bearing component)
// 2. Layer height: 0.2mm for strength
// 3. Infill: 100% for structural rigidity (clamp forces are significant)
// 4. Orientation: print with clamp jaws vertical (optimize layer strength)
// 5. Support: minimal (clamp interior only)
// 6. Post-processing: sand smooth, especially clamp inner surface
// 7. Optional: reinforce with metal insert sleeves at fastener points
// 8. Field use: test clamp on non-critical antenna boom first
// 9. Maintenance: periodically re-tighten U-bolt (vibration can loosen fasteners)
// 10. Boom compatibility:
//     - 1" round aluminum boom: standard OD = 25.4mm (works as-is)
//     - 1" square aluminum boom: fits in circular clamp (add padding)
//     - Other sizes: scale jaw radius in code (jaw_inner_radius parameter)

// TORQUE SPECIFICATION FOR U-BOLT:
// 8mm stainless steel U-bolt with M8 nuts
// Recommended torque: 12-15 N·m (avoid over-tightening aluminum boom)
// Tools needed: 13mm wrench or socket, torque wrench
