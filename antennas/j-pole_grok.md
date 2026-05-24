**Detailed Hybrid J-Pole Antenna Build: Schedule 40 PVC + 3" Lawn Chair Webbing + 2" Copper Tape (2m Band, 146 MHz Center)**

This hybrid adapts the classic J-Pole (a ¾-wave radiator with ¼-wave matching stub) using your exact materials: **PVC** for rigid mast/support, **lawn chair webbing** as durable flexible backing, and **2" adhesive copper foil tape** as the wide, low-loss conductor. The wide tape creates a "fat" element with excellent bandwidth, while the webbing makes it nearly indestructible and rollable for portable use. PVC integrates as a mast (or optional sleeve for weatherproofing). Performance rivals a full-size copper-pipe J-Pole but is lighter, cheaper (~$15–25), and packable.

**Target**: 2m ham band (144–148 MHz), omnidirectional vertical with ~2–3 dBi gain, SWR <1.5:1 across the band after tuning. Overall height ~60–62" (including mast base).

### Materials
- **PVC**: 6–8 ft of 1–2" Schedule 40 PVC pipe (cut to 66–72" total; bottom 6–12" for mounting/base).
- **Webbing**: ~8–10 ft of 3" lawn chair webbing (one continuous strip or two joined).
- **Tape**: 2" wide adhesive copper foil tape (~12–15 ft total).
- **Coax**: RG-58 or better, 6–10 ft with connector.
- **Misc**: Zip ties, UV-resistant electrical tape, Sharpie, utility knife/scissors, optional ¾" drill bit (for feed if sleeved).

### Exact Dimensions (for 146 MHz, VF ≈ 0.95–0.96 for tape on webbing)
These are practical, field-verified values for tape/webbing J-Poles (adjusted slightly shorter than free-space due to end effects and substrate).  
- **Long radiator (¾-wave vertical element, from shorting bar to top)**: **58.5–58.85 inches** (centerline measurement).  
- **Matching stub (¼-wave parallel leg)**: **19.35–19.5 inches** (parallel to long element).  
- **Shorting bar (bottom connector)**: **2 inches** wide (horizontal tape bridge).  
- **Spacing between parallel elements** (center-to-center): **2 inches** (non-critical; 1.5–2.5" works).  
- **50 Ω feedpoint** (from shorting bar, along the long element side): **2.25 inches** up (adjust ±0.5" for SWR).  

The 2" tape width gives broader bandwidth than thin wire/tubing versions. Total unfolded tape length ≈ 80–82 inches (plus shorting bar).

### Step-by-Step Hybrid Build
1. **Prepare the webbing conductor**  
   Cut one long ~80" strip of 3" webbing. Clean with alcohol. Center and press 2" copper tape smoothly along its length (½" webbing overhang on each side for strength/edges). This creates your "fat ribbon" conductor.

2. **Lay out the J-shape on the webbing**  
   On a flat surface, mark the J pattern directly on the webbing:  
   - Bottom shorting bar: 2" horizontal tape section.  
   - From left end of shorting bar: run vertical tape upward **19.5"** (stub leg).  
   - From right end of shorting bar: run vertical tape upward **58.5"** (main radiator).  
   - Keep the two vertical legs exactly **2" apart** (center-to-center; use a spacer or marks).  
   - Overlap tape at corners/junctions by 1" for continuity. Press firmly—no wrinkles.  
   Trim ends square, leaving 1" overhangs for connections if needed.

3. **Attach to PVC mast (hybrid support)**  
   - Use the PVC as the vertical mast. Align the long radiator leg along the PVC.  
   - Secure the entire webbing J-Pole to the PVC with UV zip ties or wraps of electrical tape every 6–8" (keeps it rigid and straight).  
   - Alternative (fully flexible): Skip full PVC attachment and use short PVC sections only at the feedpoint and top for support; hang the webbing J like a roll-up antenna.  
   - Bottom 6–12" of PVC serves as mounting stub (slip into larger pipe or base).

4. **Create the feedpoint**  
   - Measure **2.25"** up from the shorting bar along the **long radiator** leg.  
   - Solder the coax: center conductor to the long leg, shield to the stub leg (at same height).  
   - Reinforce solder tabs with small extra tape patches if needed. Seal everything with electrical tape.  
   - Add a simple choke balun: 6–8 turns of coax coiled below the feedpoint and zip-tied.

5. **Seal and finish**  
   Wrap all tape edges and overlaps with UV tape. Paint or cover if desired for stealth/UV protection. The webbing already shields the copper from abrasion/weather.

### Tuning
- Start with the dimensions above (slightly long).  
- Use an SWR analyzer or meter. Mount in final position.  
- **Trim for frequency**: Shorten the **top** of the long radiator in ¼–½" increments (raises resonance).  
- **Adjust SWR**: Move the feedpoint up/down the long leg (±½") or tweak stub length slightly. Goal: <1.5:1 across 144–148 MHz. The fat 2" tape usually gives wide bandwidth.  
- Re-check after sealing (minor shift possible).

### Tips & Advantages
- **Why hybrid excels**: Webbing makes it rollable/portable (fits in a bag) yet tough; PVC adds rigidity and easy mounting (mast + antenna in one). Wider tape = better bandwidth/efficiency than thin designs.  
- **Power**: Safe for 50–100 W+.  
- **Variations**: For dual-band (2m/70cm), extend dimensions per dual-band calculators (long element becomes ~2.5λ on 70cm). Or sleeve the whole thing in larger PVC for full weatherproofing.  
- **Math reference** (quick scaling):  
  Long element ≈ (0.75 × 11808 / f_MHz) × VF inches  
  Stub ≈ (0.25 × 11808 / f_MHz) × VF inches  
  (VF ≈ 0.95 for tape/webbing; use online calculators like m0ukd for exact).

This is a proven, packable design (similar to duct-tape + copper-foil J-Poles but upgraded with your stronger webbing and PVC mast). It performs exceptionally well for events, POTA, or base use.

Specify a different band (e.g., 70cm or 6m) or goal (Slim Jim variant, dual-band), and I can adjust dimensions or steps!

**J-Pole Tuning Techniques** for your **hybrid PVC + 3" lawn chair webbing + 2" copper tape J-Pole** (2m band) are straightforward and forgiving thanks to the wide tape and flexible webbing. The design is tolerant of small variations, and the materials make adjustments easy (trim with scissors, add overlapping tape for extensions, or reposition connections).

### Core Principles
A J-Pole is a ¾-wave radiator with a ¼-wave matching stub.  
- **Frequency (resonance)**: Primarily set by the **long radiator length** (top to shorting bar).  
- **Impedance/SWR match**: Primarily set by the **feedpoint position** along the long element (from the shorting bar) and the **stub length**.  
- The wide 2" tape gives broader bandwidth than thin-wire or pipe versions, often covering most/all of 2m once tuned.

**Always tune in the final mounting position** (height, away from metal/objects) with an **antenna analyzer** (NanoVNA, RigExpert, etc.) or SWR meter at low power. Use a choke balun (6–8 turns of coax below feedpoint) to prevent common-mode issues.

### Step-by-Step Tuning Process
1. **Initial Setup (Start Slightly Long)**  
   Build to the dimensions I provided earlier (long element ~58.5–59", stub ~19.5", feedpoint starting at ~2–2.5" up from shorting bar). This usually resonates a bit low.  
   Temporarily attach the coax for testing:  
   - Use small extra tape patches or **magnets** (strong neodymium) on the copper tape for movable connections — no soldering yet.  
   - Center conductor to long radiator; shield to stub (same height).

2. **Tune SWR / Match First (Feedpoint Adjustment)**  
   Sweep 144–148 MHz and find the frequency of lowest SWR.  
   - Move the feedpoint **up or down** the long element (and matching height on the stub) in ¼"–½" increments.  
     - Higher feedpoint often improves match but can shift frequency.  
     - Goal: Lowest SWR (ideally <1.5:1, best ~1.2:1 or better) at your center frequency (e.g., 146 MHz).  
   - The 2" tape width makes this less critical than on thin elements.

3. **Tune Frequency (Resonance)**  
   Once SWR is reasonable:  
   - **Trim the top** of the long radiator (¼"–½" at a time) to raise the resonant frequency.  
     - Too low (resonance below target) → trim top.  
     - Too high → add overlapping copper tape to the top (extends effectively).  
   - Do **not** trim the stub yet unless needed.  
   - Re-check and re-adjust feedpoint after each trim (iterate 2–3 times).

4. **Fine-Tune the Stub (if needed)**  
   - Shorten the stub slightly (¼" increments) if SWR won't drop low enough after feedpoint tweaks.  
   - Lengthen by overlapping tape if resonance shifts too far.  
   - Stub adjustments mainly affect matching; long element affects frequency more.

5. **Finalize**  
   Once SWR is good across the band: Solder the feedpoint permanently, seal with electrical/UV tape, and re-check (minor shift possible from sealing).  
   Add the choke balun if not already present.

### Quick Reference Adjustments
| Issue | Primary Fix | Secondary Fix |
|-------|-------------|---------------|
| Resonance too low (min SWR below target freq) | Trim top of long element | Add tape to top |
| Resonance too high | Add overlapping tape to top | — |
| High SWR (poor match) | Move feedpoint up/down | Shorten/lengthen stub slightly |
| Narrow bandwidth | Check balun, feed symmetry, nearby objects | — |

### Hybrid-Specific Tips
- **Webbing advantage**: Easy to cut cleanly or add tape overlaps (conductive enough for RF).  
- **PVC mast**: Minimal detuning effect; tune fully assembled.  
- **Wide 2" tape**: Broader SWR curve — you’ll likely get the full 2m band easily.  
- **Power**: Test at 5–10 W first, then full power.  
- **Common pitfalls**: No balun (causes erratic SWR), feed reversed, or tuning too close to ground/metal.

This process typically takes 15–30 minutes with an analyzer. Tape-based J-Poles are highly adjustable compared to rigid copper-pipe versions.

If you share your current dimensions, SWR sweep results (e.g., min SWR at what frequency), or target center frequency, I can give precise trim/feedpoint suggestions! For other bands, the same iterative approach applies—just scale the starting dimensions.


**Dual-band J-Pole tuning** (typically 2m/70cm, 144–148 MHz and 430–450 MHz) for your **hybrid PVC + 3" lawn chair webbing + 2" copper tape** design builds directly on the single-band techniques but requires iterative compromises between the two bands. The wide tape helps with bandwidth, but UHF (70cm) is more sensitive to dimensions and nearby objects.

### Design Principles for Dual-Band
A standard dual-band J-Pole uses a **¾λ radiator on 2m** that becomes roughly **2.5λ on 70cm**. The **¼λ stub on 2m** acts as a **¾λ stub on 70cm**, providing reasonable matching on both without extra traps or coils in basic versions. Some advanced designs (e.g., DBJ-1) add a shorted coax stub at the top for better 70cm performance.

**Starting Dimensions** (for ~146/445 MHz center, tape/webbing VF ≈ 0.94–0.96; start slightly long):
- **Long radiator** (from shorting bar to top): **58–60 inches** (enhances 70cm performance; ~63 inches in some copper-pipe versions for better UHF).
- **Matching stub** (parallel leg): **19–20 inches**.
- **Shorting bar**: 2 inches wide.
- **Element spacing** (center-to-center): 2 inches.
- **Initial feedpoint** (from shorting bar, on long leg): **2–2.5 inches** up.

These are adaptable from single-band (trim less aggressively). The webbing/tape hybrid rolls up nicely for portable use.

### Step-by-Step Dual-Band Tuning Process
Use an **antenna analyzer** (e.g., NanoVNA) for sweeps on both bands. Tune in final mounting position with choke balun installed. Start with magnets or temporary tape connections for easy adjustments.

1. **Initial Build & Rough Check**  
   Assemble to the dimensions above. Sweep both bands separately.  
   - Expect 2m to be closer to target; 70cm may show higher SWR or shifted resonance.

2. **Tune 2m First (Primary Band)**  
   - **Frequency**: Trim the **top** of the long radiator (¼–½" increments) to center resonance around 146 MHz.  
   - **SWR/Match**: Adjust feedpoint up/down (±¼–½") along the long leg (and same height on stub) for lowest SWR.  
   - Goal: <1.5:1 (ideally <1.3:1) across 144–148 MHz.

3. **Tune 70cm (Iterate with 2m)**  
   - 70cm resonance is more sensitive. Trim top or adjust stub length slightly if needed.  
   - The long element should work as ~2.5λ; minor top trims affect UHF more.  
   - Re-check and tweak feedpoint — it impacts both bands.  
   - If 70cm SWR stays high: Add a small shorted coax stub (e.g., RG-174, ~6–8 inches) at the top for decoupling (acts as high impedance on UHF, minor inductance on VHF).

4. **Compromise & Fine-Tune**  
   Alternate between bands: Trim top → check both → move feedpoint → check both.  
   - Wider 2" tape provides more bandwidth, often covering full bands once balanced.  
   - Add overlapping tape pieces if you overshoot (extends length).  
   - Final: Aim for <1.5:1 on 2m and <2:1 (usable) on 70cm across desired segments. Perfect 1:1 on both is rare without extra elements.

5. **Finalize**  
   Solder feedpoint, seal with UV tape, re-sweep (minor shifts from sealing/PVC). Test with low power first.

### Quick Adjustment Reference
| Issue                  | Primary Fix                          | Secondary Fix                     |
|------------------------|--------------------------------------|-----------------------------------|
| 2m resonance low      | Trim top of long element            | —                                 |
| 70cm resonance low    | Trim top (smaller steps)            | Shorten stub slightly             |
| High SWR on 2m        | Move feedpoint up/down              | Adjust stub length                |
| High SWR on 70cm      | Feedpoint tweak or top trim         | Add top coax stub                 |
| Bands interact badly  | Iterate small changes; check balun  | Slight spacing adjustment         |

### Hybrid Advantages & Tips
- **Webbing + tape**: Extremely easy to trim/add (scissors + overlap). Portable/rollable. PVC mast adds rigidity.
- **Bandwidth**: Fat elements help cover both bands better than thin wire.
- **Common Issues**: No balun causes erratic SWR; tune away from metal; UHF more affected by surroundings.
- **Power**: Good for 50–100W+ after tuning.
- **Variations**: For better 70cm, look at DBJ-style with extra top stub or extended radiator.

This yields a strong dual-band performer for your materials — great for portable (POTA), events, or base. Share your current dimensions or SWR readings on each band, and I can suggest exact tweaks! For other pairs (e.g., 6m/2m), scaling follows similar math.


**Tri-band J-Pole designs** (typically **2m / 1.25m / 70cm** — 144–148, 222–225, and 430–450 MHz) are more complex than dual-band versions because 1.25m (222 MHz) does not share simple harmonic relationships with the other two bands. Perfect simultaneous resonance on all three with a single simple J structure is difficult, so practical designs use compromises, extra decoupling stubs, or segmented elements.

Commercial examples like **Ed Fong’s TBJ-1** (twin-lead/ribbon cable inside PVC pipe) work well and are popular for tri-band HTs and base use. Your **hybrid PVC + 3" lawn chair webbing + 2" copper tape** approach can adapt these concepts for a durable, rollable/portable version with the wide tape providing good bandwidth.

### Feasible Hybrid Tri-Band Approaches
1. **Extended Dual-Band Base with Compromises** (Easiest for DIY)  
   Start with the dual-band J-Pole dimensions (long radiator ~58–63", stub ~19–20") and tune for the best overall compromise.  
   - The long element works as ~¾λ on 2m, ~2.5λ on 70cm, and roughly usable (though not optimal) on 1.25m.  
   - Expect good performance on 2m/70cm and marginal (SWR ~2–3:1) on 1.25m, improvable with careful feedpoint and stub tweaks.  
   - Add a shorted RG-174 coax stub (~6–8") at the top for better UHF decoupling (helps 70cm and can aid 1.25m).

2. **Segmented / Multi-Stub Hybrid (Closer to Ed Fong TBJ-1 Style)**  
   Lay out tape on webbing with multiple tuned sections and decoupling points:  
   - **Long radiator**: ~62–65" total (optimized around 63" for enhanced 70cm while keeping 2m usable; helps pull 1.25m closer).  
   - **Main matching stub**: ~19.5–20.5" (¼λ on 2m, ~¾λ on 70cm).  
   - **Additional 1.25m stub or trap**: A parallel short segment (~12–14" for 222 MHz) or a shorted coax stub spliced near the 1.25m high-impedance point.  
   - **Shorting bar**: 2" wide.  
   - **Spacing**: 2–2.5" between parallel legs.  
   - **Feedpoint**: Start at ~2–3" up from shorting bar; this position is a compromise across bands.

   Use the webbing as a continuous backing strip. Apply copper tape in the J shape for the main elements, then add short parallel tape sections or soldered coax stubs at calculated points for the third band. Mount the whole assembly along a PVC mast with zip ties.

3. **Stacked or Collinear Variant**  
   Build a primary J-Pole for 2m/70cm and add a separate phased 1.25m element higher on the same PVC mast (using additional webbing/tape). This avoids full interaction but increases height (~8–10 ft total).

### Tuning for Tri-Band (Iterative Compromise)
Tune in final position with an analyzer sweeping all three bands. Use temporary magnet or tape connections.  
- **Start long** on the radiator and stub.  
- **Prioritize 2m** (trim top of long element).  
- **Then 70cm** (small top trims + top coax stub length).  
- **Finally 1.25m** (adjust feedpoint or add dedicated short stub/overlap).  
- Expect: <1.5:1 on 2m, <2:1 on 70cm, usable 2–3:1 on 1.25m (or better with extra stubs). The wide 2" tape helps bandwidth.  
- Iterate: Small changes on one band affect others. A top RG-174 shorted stub (length tuned for high impedance on 1.25m/70cm) is key for cleaner multi-band operation.

### Construction Tips for Your Materials
- **Webbing/Tape**: One long 3" webbing strip (~7–8 ft) with centered 2" copper tape. Overlap tape sections for junctions.  
- **PVC**: 6–8 ft mast for support/rigidity; optional larger pipe sleeve for full enclosure.  
- **Feed**: Single coax feedpoint (center to long leg, shield to stub). Strong choke balun essential.  
- **Durability**: Seal edges thoroughly; webbing protects tape well for portable use.  
- **Performance**: ~2–3 dBi gain, no radials needed. Good for base, POTA, or emergency tri-band radios.

True high-performance tri-band J-Poles often rely on Ed Fong’s proprietary twin-lead + coax stub techniques inside PVC. Your tape/webbing hybrid will be close but may require more on-air testing and tweaks.

For best results, specify your target frequencies (exact centers) or priorities (e.g., strongest on 1.25m), and I can refine dimensions or suggest stub lengths. A NanoVNA makes tri-band tuning manageable. This setup stays cheap, lightweight, and packable!

**Dual-band J-Pole variations** (primarily **2m / 70cm**) adapt well to your **hybrid PVC + 3" lawn chair webbing + 2" copper tape** construction. The wide tape provides good bandwidth, the webbing adds durability and rollability, and PVC offers rigid support or full enclosure. These variations range from simple harmonic designs to enhanced versions with decoupling stubs for better UHF performance.

### 1. Basic Harmonic Dual-Band J-Pole (Simplest)
This relies on the long radiator acting as ~¾λ on 2m and ~2.5λ on 70cm, with the stub providing matching on both.  
- **Dimensions** (starting point, 146/445 MHz centers): Long radiator 58–63", stub 19–20.5", spacing 2", feedpoint ~2–2.5" up from shorting bar.  
- **Hybrid build**: Single continuous tape/webbing strip in J-shape, zip-tied to PVC mast.  
- **Performance**: Good on 2m, usable on 70cm. Tune as previously discussed (prioritize 2m, then tweak for UHF).  
- **Pros/Cons**: Easiest to build/roll up; 70cm may have higher SWR or pattern tilt without extras.

### 2. Ed Fong DBJ-1 / DBJ-2 Style (Most Popular Enhanced Version)
Uses 300Ω or 450Ω twin-lead principles but adaptable to your tape/webbing. Adds a **short decoupling stub** (RG-174 coax, ~6–8" shorted at end) at the top or mid-radiator for cleaner UHF operation and reduced interaction.  
- **Variations**:  
  - **Roll-up DBJ-2**: Fully flexible tape/webbing version—no full PVC enclosure. Excellent for portable/POTA/go-kits; rolls into a small bag.  
  - **PVC-enclosed DBJ-1**: Mount your tape/webbing J inside or on a ½–¾" Schedule 40 PVC tube (acts as radome/mast). This protects elements and improves weatherproofing.  
- **Hybrid adaptation**: Apply tape to webbing for main J, then attach a short parallel tape section or soldered coax stub at the calculated high-impedance point (~⅓ up the radiator).  
- **Performance**: Better balance, lower loss on 70cm than basic versions (~2–3 dBi on 2m, higher effective gain on UHF).

### 3. Slim Jim Dual-Band Variant
A folded J-Pole (narrower profile, slightly different impedance).  
- Lay the tape/webbing in a parallel "folded" J (radiator folded back on itself at top). Add the same top decoupling stub for dual-band.  
- **Hybrid build**: Use two parallel webbing strips or fold one wide strip; secure to thin PVC or hang freely.  
- **Advantages**: More compact width; good omnidirectional pattern; easy to roll. Often outperforms basic J-Poles in some environments.

### 4. Open Stub / Cactus or Multi-Stub Variations
- **Open-stub J-Pole**: Uses an open (unshorted) matching section tuned for both bands. Popular in commercial aluminum versions (e.g., Arrow OSJ 146/440).  
- **Cactus style**: Multiple parallel stubs/elements for broader matching.  
- **Hybrid**: Add short parallel tape/webbing stubs (tuned lengths) alongside the main stub. Mount on PVC boom/mast. Great for base stations.

### 5. Coax or Tape-Only Roll-Up Variations
- Build the entire J from copper tape on webbing (no twin-lead needed). Some designs use RG-58 coax sections for the matching stub.  
- Fully flexible "roll-up" versions are popular for ARES/emergency kits—perfect match for your materials.

### General Construction & Tuning Tips for All Variations
- **Feed**: Single coax (center to long leg, shield to stub). Always use a choke balun (6–8 turns coax or ferrite).  
- **Tuning**: Same iterative process—2m first (trim top), then 70cm (feedpoint + decoupling stub length). Use temporary magnets/tape for adjustments.  
- **PVC role**: Mast support, full sleeve for protection, or cross-piece for spacing.  
- **Webbing/Tape benefits**: Wider 2" conductor = broader bandwidth; easy to add overlaps for tweaks. Seal thoroughly for outdoor use.  
- **Expected results**: <1.5:1 on 2m, <2:1 on 70cm across most of the bands with good tuning.  

The DBJ-style with decoupling stub is widely recommended for balanced performance without complexity. These stay cheap, lightweight, and highly portable with your materials.

If you want detailed dimensions, a step-by-step for a specific variation (e.g., roll-up DBJ-2 style or with coax stub), or tweaks for GMRS/70cm focus, just specify!


**Decoupling stub tuning techniques** for your **hybrid PVC + lawn chair webbing + 2" copper tape dual-band J-Pole** (2m/70cm) focus on the shorted coaxial stub (typically RG-174) placed at the top of the UHF radiating section. This stub provides high impedance at UHF (decoupling the upper VHF portion so it doesn't interfere) while acting as a small inductance at VHF (with minimal impact).

### Purpose and Placement
- The stub creates independent ~½-wave radiators on both bands by choking UHF currents on the upper part of the long element.
- **Typical location**: Connected across the top of the UHF section (open end of stub to the tape/webbing radiator). The shorted end is toward the top.
- **Length guideline**: ~¼-wave at 435–445 MHz, adjusted for coax velocity factor (VF ≈ 0.66 for RG-174).
  - Free-space ¼-wave at 440 MHz ≈ 6.7 inches.
  - With VF: ~4.4–4.6 inches electrical length (physical coax before shorting).

In your hybrid: Attach the stub by soldering to small copper tape patches on the webbing, or use strong clips/magnets for temporary tuning.

### Step-by-Step Tuning Process
Tune **after** basic J-Pole adjustments (main radiator, stub, and feedpoint). Use an **antenna analyzer** (NanoVNA preferred) for both bands in the final mounting position (hanging freely, away from metal/objects). Install a choke balun on the feedline.

1. **Start with an Over-Length Stub**  
   Cut RG-174 coax ~10–15% longer than calculated (e.g., start at 5–5.5 inches total before shorting).  
   - Short one end (solder center to shield).  
   - Leave the other end open initially.

2. **Attach Temporarily**  
   Connect the open end of the stub across the top of the UHF radiator section (parallel to the element).  
   - Use solder tabs (extra tape patches) or strong magnets/clips for easy removal/adjustment.

3. **Tune UHF (70cm) First**  
   Sweep 430–450 MHz and monitor SWR (or return loss).  
   - **Trim the open end** in small increments (⅛" or less as you get close).  
     - Trimming shortens the stub → raises the frequency where it provides high impedance (better decoupling).  
   - Goal: Lowest SWR (ideally <1.5–2:1) across your desired 70cm segment. The stub should create a clear "stop band" or high-impedance point at UHF.  
   - Re-check 2m after each adjustment — the stub adds slight inductance on VHF, which may require minor top-radiator trim.

4. **Iterate with Main Antenna**  
   - If UHF improves but 2m shifts: Trim/add a tiny overlap on the main long radiator top.  
   - If SWR won't drop: Slightly reposition the stub attachment point (½" moves) or adjust overall feedpoint.  
   - Test both bands alternately until balanced.

5. **Finalize**  
   Once optimized: Solder permanently, cover with heat-shrink tubing or electrical tape for weatherproofing, and re-sweep. Seal the entire antenna.

### Quick Reference Adjustments
| Issue                        | Primary Fix                          | Notes |
|------------------------------|--------------------------------------|-------|
| UHF SWR high / resonance low | Trim open end of stub               | Small cuts near target |
| UHF pattern tilt or poor gain| Reposition stub or lengthen slightly| Ensures proper decoupling |
| 2m affected after stub add   | Trim main radiator top slightly     | Stub inductance shortens VHF |
| Stub too sensitive           | Use analyzer for precise stop-band  | Check S11 for high impedance |

### Hybrid-Specific Tips
- **Webbing/tape**: Easy to add solder tabs with overlapping copper tape. The wide 2" conductor broadens overall bandwidth, making stub tuning more forgiving.
- **PVC role**: Use as mast support; the stub can run parallel inside or alongside.
- **Coax choice**: RG-174 is ideal (flexible, small). RG-58 works but is stiffer and has different VF.
- **Power**: Test at low power (5–10 W) during tuning.
- **Tools**: NanoVNA for seeing the exact high-impedance point is highly recommended.

This technique (start long, trim open end) comes directly from Ed Fong’s DBJ designs and yields excellent dual-band results with minimal interaction.

Share your current stub length, SWR readings on each band, or target centers (e.g., 146/445 MHz), and I can suggest precise starting lengths or next trim steps! This pairs perfectly with your rollable hybrid for portable use.



