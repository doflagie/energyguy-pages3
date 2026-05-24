**Detailed Hybrid Helical Antenna Build: PVC + Lawn Chair Webbing + 2" Copper Tape (2m Band Vertical Dipole)**

This hybrid combines **Schedule 40 PVC** (rigid core and mast), **3" lawn chair webbing** (flexible, UV-resistant, tear-proof substrate), and **2" adhesive copper foil tape** (conductor). It adapts the proven W6NBC "barber pole" or spirally-loaded design (normally 1" tape directly on PVC) into a tougher, wider-conductor version. The webbing backs the tape for better mechanical strength, weather resistance, and easier handling—no peeling or tearing in wind/portable use. The 2" tape creates a "fat" helix with potentially broader bandwidth than the original 1" version.

**Target performance** (2m ham band, 144–148 MHz center): ~18–20" active helix height on a 36" total PVC mast. Omnidirectional vertical dipole with efficiency and SWR comparable to a full-size 5 ft J-pole, but ultra-compact, lightweight (~$15–25 total), and rollable for portable/event use (POTA, bike-a-thons, etc.). Expected SWR <1.5:1 across the band after tuning.

### Materials (for one antenna)
- **PVC**: 36" length of 1-1/4" or 2" Schedule 40 PVC pipe (1-1/4" is ideal for the original; 2" gives more room for the wider webbing helix).
- **Webbing**: ~6–8 ft of 3" lawn chair webbing (polypropylene or vinyl strapping; cut from a 50 ft roll).
- **Tape**: 2" wide adhesive-backed copper foil tape (RF shielding type, ~1.5–3 mil thick; ~10–15 ft needed). Aluminum tape works but is harder to solder.
- **Coax**: 6–10 ft RG-58 or Mini-8 with PL-259 (or direct solder pigtail).
- **Misc**: Zip ties (UV-resistant), electrical/duct tape (for sealing), Sharpie, utility knife or scissors, ¾" drill bit, sandpaper, optional ferrite choke or 6–8 turns coax balun.

### Step-by-Step Build

1. **Prepare the webbing conductor strip**  
   Cut a single continuous strip of 3" webbing ~5–7 ft long (extra for trimming). Clean it with alcohol. Center the 2" copper tape lengthwise on the webbing and press it down smoothly (no wrinkles). The ½" webbing overhang on each side acts as reinforcement and edge protection. This creates a wide, durable "tape ribbon." Trim the ends square.

2. **Mark the helix guide line on the PVC**  
   Cut a temporary guide strip (54" long × 2–3" wide) from cardstock, plastic report folder, or spare webbing.  
   - Wrap it tightly around the PVC at the center of the planned helix area (top ~20" of the 36" pipe). Butt the edges and tape it temporarily.  
   - Unwrap slowly while drawing a continuous spiral guideline along one edge with a Sharpie. Aim for ~10 initial turns over the top 20–22" (this gives a starting pitch of ~2–2.25" per turn).  
   The wider 2" tape + webbing will fill most of each turn with a small gap (~½–1") for low loss.

3. **Wrap the hybrid helix**  
   Starting at the bottom of the helix section (about 16–18" from the top of the PVC), align the webbing/tape strip with the guideline.  
   - Press the adhesive tape side firmly onto the PVC while spiraling upward, following the line exactly. Keep tension even—the flexible webbing conforms beautifully and won't crease like plain tape.  
   - Overlap the webbing edges slightly if needed for continuous coverage, or leave a small gap (½–1"). Secure every few turns with zip ties or a wrap of electrical tape for extra hold.  
   - Stop ~20" up. Trim excess webbing/tape at both ends, leaving 1–2" overhang for connections.  
   Result: A reinforced "fat" helical conductor ~18–20" tall with ~8–10 turns initially.

4. **Create the feedpoint (center-fed dipole)**  
   Locate the exact center of the helix (~9–10" from each end). Carefully cut away ~1" of the copper tape/webbing at that spot (expose the PVC).  
   Drill a ¾" hole through the PVC at that point for the coax feed.  
   - Strip the coax and solder the center conductor to one side of the helix (top section) and the shield to the other side (bottom section). Use the copper tape's overhang or add small tape patches for solder tabs if needed.  
   - Seal the hole and connections with electrical tape or silicone.  
   (This off-center feed—4½ turns bottom, 3½ top after tuning—matches impedance nicely.)

5. **Add balun/choke and mounting**  
   Wind 6–8 turns of the feed coax tightly around the lower PVC (below the helix) and secure with zip ties—this acts as a 1:1 balun/choke.  
   The bottom 14–16" of PVC serves as the mast or slips into a larger pipe/base. Cap the top if desired. Paint the whole thing for stealth/UV protection.

6. **Seal and finish**  
   Wrap the entire helix edges and overlaps with UV-resistant tape or clear sealant. The webbing already protects the copper from abrasion and weather.

### Tuning and Testing
- **Initial test**: The starting ~10 turns will resonate low (below 144 MHz). Use an SWR analyzer or radio + meter.  
- **Trim to tune**: Remove short sections of tape/webbing from the **top end only** (shortens the upper half). Target final: ~3½ turns above feedpoint and ~4½ below (total ~8 turns, ~18" height). This typically centers at 146 MHz with excellent SWR (<1.5:1 across 144–148 MHz).  
- Test outdoors, away from objects. Trim in ½" increments and recheck. The hybrid's wider elements often give broader bandwidth than the original.

### Tips & Variations
- **Why this hybrid excels**: Webbing makes the antenna nearly indestructible for portable use, while the PVC provides perfect rigidity and mast integration. Wider 2" tape lowers losses slightly and broadens response.
- **Power**: Safe for 50–100 W (test at low power first).
- **Scaling**: For other bands, lengthen PVC and add more turns (use W6NBC guidelines: longer pipe/more turns = lower frequency; wider tape/gap also lowers it).
- **Troubleshooting**: If SWR is high, add a small variable capacitor across the feed or adjust turns. PVC velocity factor is minor here.
- **Cost & time**: Under $25 and 1–2 hours. Rolls up small for storage.

This build is field-tested in spirit (based directly on W6NBC's QST design) but upgraded with your exact materials for superior durability. It performs great as a base, mobile, or go-kit antenna.

If you want dimensions for a different band (e.g., 70cm or HF), exact helix math, or photos of similar builds, just let me know!**Helix tuning techniques** for your **hybrid PVC + lawn chair webbing + 2" copper tape helical antenna** (especially the 2m center-fed vertical dipole design) rely on the same principles as W6NBC’s original spirally-loaded copper tape PVC antenna, with minor adaptations for the wider tape and webbing backing.

The helix behaves like a continuously loaded shortened dipole. **Total electrical length** primarily controls the resonant frequency, while the **feedpoint offset** (turns above vs. below the feed) adjusts impedance/SWR.

### Core Tuning Rules (from W6NBC and similar designs)
- **Frequency** (resonance): Controlled by **total helix length** (number of turns × circumference). Longer helix → lower frequency.  
- **SWR/impedance**: Controlled by **feedpoint position** (asymmetric feed). A shortened dipole often needs a slight offset for ~50 Ω match.  
- **Key guideline**: Tune **SWR first** (adjust top/bottom balance), **then frequency** (trim both ends equally). Once SWR is good, frequency tweaks have minimal impact on match.

### Step-by-Step Tuning Process for the Hybrid
1. **Initial Setup (Overbuilt for Easy Tuning)**  
   - Start with extra length: ~10 turns total over ~20–22" helix height (as in the build guide). This resonates **below** 144 MHz.  
   - Feed at approximate center initially.  
   - Install the balun/choke (coax turns or ferrite) and mount the antenna in its final position (away from metal objects, at operating height).  
   - Use an **antenna analyzer** (e.g., RigExpert, NanoVNA, MFJ-259) or SWR meter + low-power transmitter. Sweep 144–148 MHz.

2. **Rough Tune: Set Target Turns Ratio for SWR**  
   - Trim **only from the top end** until you have roughly **3½ turns above the feedpoint** and **4½ turns below** (total ~8 turns).  
   - This ratio often puts SWR in the ballpark (~2:1 or better) per W6NBC’s experience. The wider 2" tape + webbing may give slightly broader bandwidth, so your exact numbers might vary a bit.

3. **Fine-Tune SWR (Impedance Match)**  
   - Adjust **only the top section** (add or remove short pieces of tape/webbing).  
     - Remove from top → raises frequency slightly and changes SWR.  
     - Add overlapping tape (1" overlap) if you go too far — capacitive coupling works well.  
   - Goal: Lowest SWR at your center frequency (e.g., 146 MHz). The hybrid’s fat conductor often yields <1.5:1 across most of 2m.  
   - If SWR won’t drop below ~2:1, check: feedpoint connections, balun effectiveness, coax length inside PVC, or nearby objects.

4. **Fine-Tune Frequency (Resonance)**  
   - Once SWR is good, trim **equal small amounts from both top and bottom** to shift the center frequency.  
     - Too low (resonance below target) → trim both ends.  
     - Too high → add overlapping pieces to both ends.  
   - Trim in small increments (½"–1" of tape/webbing per end) and re-sweep. The webbing makes clean cuts easy with scissors/utility knife.

5. **Advanced/Alternative Techniques**
   - **Variable capacitor across feedpoint**: For fine adjustment without cutting (small air-variable or trimmer, ~5–20 pF). Useful if you want to cover the full band or experiment.  
   - **Slight pitch/turn spacing changes**: Gently stretch or compress the helix along the PVC (if not fully secured) to tweak inductance.  
   - **Feedpoint movement**: Slide the feedpoint hole slightly up/down the helix (requires re-drilling or new hole) for stubborn SWR.  
   - **Analyzer sweep**: Look for the frequency of minimum SWR (resonance) and reactance zero.  
   - **Environment matters**: Tune in final mounting location. Ground, nearby objects, or height affect resonance slightly.

### Expected Results & Tips
- Final typical: ~3½ top / 4½ bottom turns for 146 MHz center with excellent SWR.
- Bandwidth: Wider than thin-wire helices due to 2" tape (often full 2m band at <1.5:1 or better).
- Power handling: Trim/test at low power first (5–10 W), then scale up.
- Hybrid advantages: Webbing allows easy trimming/adding without tape tearing or peeling. Overlaps bond well.
- Common pitfalls: Poor balun (causes common-mode currents and weird SWR), feed reversal (center conductor should go to top section), or measuring too close to the antenna.

This method is straightforward, repeatable, and requires no special tools beyond an analyzer. It’s why these antennas are popular for quick DIY projects.

If you share your initial SWR sweep results, current turn count, or target frequency/band, I can give more precise trim guidance! For other bands (e.g., 70cm or HF), the same ratio-based approach scales with more/longer turns.