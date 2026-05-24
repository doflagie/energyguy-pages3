// INTEGRATED ANTENNA ANALYZER - DISPLAY BEZEL FOR 3.5" TFT
// Anti-glare frame with touch button access windows
// Mounts to main enclosure via snap-fit tabs

// TFT display dimensions (physical panel)
tft_width = 86;      // mm, active display area ~79mm
tft_height = 65;     // mm, active display area ~57mm
tft_thickness = 2.5; // PCB thickness

// Bezel frame dimensions
bezel_thickness = 2.5;     // Material thickness
bezel_width_outer = tft_width + 8;   // 4mm margin on each side
bezel_height_outer = tft_height + 8; // 4mm margin on top/bottom
bezel_depth = 12;                    // Recessed frame depth (anti-glare lip)

// Anti-glare features
lip_height = 3;     // 3mm lip to shade screen from side reflections
lip_width = 2;      // Thin lip reduces bulk

// Touch button access (capacitive buttons on PCB behind display)
button_window_width = 8;   // mm, hole width
button_window_height = 6;  // mm, hole height
button_count = 3;          // Three touch buttons (menu, up, down)

module bezel_frame() {
  // Outer frame (with anti-glare lip)

  // Main bezel ring
  linear_extrude(height = bezel_thickness) {
    difference() {
      square([bezel_width_outer, bezel_height_outer], center = true);
      square([tft_width - 4, tft_height - 4], center = true);  // Display opening
    }
  }

  // Anti-glare lip (extends inward and upward from bezel edges)
  translate([-(bezel_width_outer/2 + lip_width), -(bezel_height_outer/2 + lip_width), bezel_thickness])
    cube([bezel_width_outer + 2*lip_width, lip_width, lip_height]);  // Left lip

  translate([-(bezel_width_outer/2 + lip_width), bezel_height_outer/2, bezel_thickness])
    cube([bezel_width_outer + 2*lip_width, lip_width, lip_height]);  // Right lip

  translate([-(bezel_width_outer/2), -(bezel_height_outer/2 + lip_width), bezel_thickness])
    cube([lip_width, bezel_height_outer + 2*lip_width, lip_height]);  // Top lip

  translate([bezel_width_outer/2 - lip_width, -(bezel_height_outer/2 + lip_width), bezel_thickness])
    cube([lip_width, bezel_height_outer + 2*lip_width, lip_height]);  // Bottom lip
}

module touch_button_windows() {
  // Three rectangular windows for capacitive touch buttons
  // Buttons positioned along bottom edge of TFT

  window_spacing = (tft_width - 30) / 2;  // Evenly spaced
  window_y = -tft_height/2 + 8;           // 8mm from bottom

  // Left button (MODE / UP)
  translate([-(bezel_width_outer/4), window_y, -0.5])
    cube([button_window_width, button_window_height, bezel_thickness + 1], center = true);

  // Center button (SELECT / MENU)
  translate([0, window_y, -0.5])
    cube([button_window_width, button_window_height, bezel_thickness + 1], center = true);

  // Right button (DOWN / ENTER)
  translate([bezel_width_outer/4, window_y, -0.5])
    cube([button_window_width, button_window_height, bezel_thickness + 1], center = true);
}

module snap_fit_tabs() {
  // Snap-fit tabs that grip recessed opening in main enclosure
  // Four tabs (corners) ensure secure mounting without screws

  tab_width = 8;
  tab_depth = 6;      // Depth of snap engagement
  tab_height = 3;     // Thin for snap flexibility

  // Positions: four corners of bezel
  corner_x = (bezel_width_outer / 2) - 5;
  corner_y = (bezel_height_outer / 2) - 5;

  // Top-left tab
  translate([-(corner_x - tab_width/2), (corner_y - tab_height/2), -tab_depth])
    cube([tab_width, tab_width, tab_height]);

  // Top-right tab
  translate([(corner_x - tab_width/2), (corner_y - tab_height/2), -tab_depth])
    cube([tab_width, tab_width, tab_height]);

  // Bottom-left tab
  translate([-(corner_x - tab_width/2), -(corner_y - tab_height/2), -tab_depth])
    cube([tab_width, tab_width, tab_height]);

  // Bottom-right tab
  translate([(corner_x - tab_width/2), -(corner_y - tab_height/2), -tab_depth])
    cube([tab_width, tab_width, tab_height]);
}

module mounting_boss_for_encoder() {
  // Small boss to support rotary encoder shaft on back side

  boss_radius = 4;
  boss_height = 8;
  boss_x = (bezel_width_outer / 2) - 12;  // Slightly inset from edge

  translate([boss_x, 0, -boss_height])
    cylinder(h = boss_height, r = boss_radius, $fn = 32);

  // Small hole for encoder shaft (3mm)
  translate([boss_x, 0, -boss_height - 1])
    cylinder(h = boss_height + 2, r = 1.5, $fn = 32);
}

module cable_access_slot() {
  // Slot for flat ribbon cable to LCD controller
  // Positioned at bottom-center of bezel (cable runs down the middle)

  slot_width = 10;   // 10mm wide (typical FFC cable width)
  slot_height = 3;   // Thin slot
  slot_length = tft_height / 2;

  translate([-(slot_width/2), -(slot_height/2), -10])
    cube([slot_width, slot_height, 10]);
}

module assembly() {
  // Main assembly (union of all parts)
  union() {
    difference() {
      // Base bezel frame
      bezel_frame();

      // Remove touch button windows
      touch_button_windows();

      // Remove cable access slot
      cable_access_slot();
    }

    // Add snap-fit tabs
    snap_fit_tabs();

    // Add encoder mounting boss (optional, for encoder alignment)
    mounting_boss_for_encoder();
  }
}

// RENDER THE ASSEMBLY
assembly();

// Translate for viewing/printing
translate([0, 0, 15])
  assembly();

// NOTES FOR MANUFACTURING:
// 1. Print in translucent PETG or resin for visibility without full transparency
// 2. Orientation: print with face UP to minimize supports
// 3. Layer height: 0.1mm for smooth display appearance
// 4. Post-processing: sand with 220-400 grit for optical clarity
// 5. Optional: apply matte acrylic spray to reduce glare (5cm distance, light coat)
// 6. Assembly: Snap frame into main enclosure recessed opening
// 7. Adhesive: Optional silicone gasket to prevent dust ingress (not essential)
// 8. Cable routing: FFC ribbon cable runs through bottom slot
// 9. Buttons: Touch sensor buttons are behind window (no mechanical buttons needed)
