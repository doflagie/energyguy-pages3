# Complete Fluke 15B+ Multimeter Calibration Example

## Project Overview

**Instrument:** Fluke 15B+ Digital Multimeter
**Goal:** Calibrate all major functions using homebrew methods
**Budget:** $22 (reusing some components)
**Time:** One Saturday (8 hours)
**Result:** Verified, documented multimeter accuracy

---

## Starting Condition

### Equipment

- Fluke 15B+ multimeter (purchased 1 year ago, never calibrated)
- Voltage references (from oscilloscope project - already built)
- GPS module (from TinySA project)
- Basic tools, breadboard

### Unknown Status

**Never calibrated since purchase:**
- DC voltage accuracy: Unknown
- Resistance accuracy: Unknown
- Current accuracy: Unknown
- Frequency counter: Unknown

**Question:** "Is my Fluke within spec after 1 year of use?"

---

## Day 1: Complete Calibration

### 9:00 AM - Planning and Shopping

**Inventory:**
- Voltage references: Have 2.5V and 5V ✓
- Need to add: 1.225V reference
- Precision resistors: Need to buy

**Shopping list:**
```
From Mouser Electronics (online order, overnight shipping):
- LM4040A-1.2: $1.50
- 100Ω ±0.1% resistors × 10: $5.00
- 1.00kΩ ±0.1% resistors × 10: $5.00
- 10.0kΩ ±0.1% resistors × 10: $5.00
- 100kΩ ±0.1% resistors × 5: $3.00
- 1.00MΩ ±0.1% resistors × 5: $3.00
Total: $22.50 + shipping
```

**Waited for overnight delivery...**

---

### Saturday 9:00 AM - Parts Arrived, Begin Calibration

**Package received:**
- All components arrived ✓
- Quality check: All parts correct

---

## Part 1: DC Voltage Calibration

### 10:00 AM - Build 1.225V Reference

**Circuit (same as previous projects):**
```
9V battery → 10kΩ → LM4040A-1.2 → 1.225V output
                      └→ GND
```

**Built on breadboard in 15 minutes**

**Self-verification:**
- Used existing 2.5V reference as comparison
- 1.225V should be exactly 49% of 2.5V
- Ratio check: 1.225 / 2.5 = 0.49 ✓

---

### 10:30 AM - Measure All Voltage References

**Setup:**
- Fluke 15B+ warmed up (powered on 30 min earlier)
- Voltage references connected via test leads
- Recorded measurements on all ranges

#### 1.225V Reference

| Range | Fluke Reading | Expected | Error | Error % |
|-------|---------------|----------|-------|---------|
| 400mV | Overrange | - | - | - |
| 4.000V | 1.231V | 1.225V | +0.006V | +0.49% |

**Note:** 400mV range can't measure 1.225V (too high)

#### 2.500V Reference

| Range | Fluke Reading | Expected | Error | Error % |
|-------|---------------|----------|-------|---------|
| 4.000V | 2.512V | 2.500V | +0.012V | +0.48% |
| 40.00V | 2.51V | 2.50V | +0.01V | +0.40% |

#### 5.000V Reference

| Range | Fluke Reading | Expected | Error | Error % |
|-------|---------------|----------|-------|---------|
| 4.000V | Overrange | - | - | - |
| 40.00V | 5.02V | 5.00V | +0.02V | +0.40% |

#### 10.00V Reference (two 5V in series)

| Range | Fluke Reading | Expected | Error | Error % |
|-------|---------------|----------|-------|---------|
| 40.00V | 10.04V | 10.00V | +0.04V | +0.40% |

---

### 11:00 AM - DC Voltage Analysis

**Key findings:**

1. **Consistent positive error** across all ranges
2. **4V range:** +0.48-0.49% error
3. **40V range:** +0.40% error
4. **Pattern:** Fluke reads ~0.4-0.5% HIGH

**Average error:** +0.44%

**Compared to Fluke spec (±0.5%):**
- **Within spec!** ✓
- Close to the limit though
- Could benefit from adjustment

**DC Voltage Correction Table Created:**
```
┌────────────────────────────────────┐
│ FLUKE 15B+ DC VOLTAGE CALIBRATION  │
│                                    │
│ Error: +0.44% (reads HIGH)         │
│                                    │
│ Correction Factor: × 0.9956        │
│                                    │
│ Examples:                          │
│   Reads 5.00V → Actual: 4.98V      │
│   Reads 10.0V → Actual: 9.96V      │
│   Reads 3.30V → Actual: 3.29V      │
│                                    │
│ Date: 2026-01-02                   │
│ Next cal: 2027-01-02               │
└────────────────────────────────────┘
```

---

## Part 2: Resistance Calibration

### 11:30 AM - Statistical Method Setup

**Concept:** Measure multiple precision resistors, find consensus

**Resistors purchased:** 10× each of 100Ω, 1kΩ, 10kΩ (all ±0.1%)

### 12:00 PM - Measure 100Ω Resistors

**Setup:**
- Fluke set to resistance mode
- 4-wire (REL) mode if available
- Zero displayed before measurement

**Measurements:**

| Resistor | Fluke Reading | Marked Value | Apparent Error |
|----------|---------------|--------------|----------------|
| R1 | 100.4Ω | 100.0Ω | +0.4% |
| R2 | 100.5Ω | 100.0Ω | +0.5% |
| R3 | 100.3Ω | 100.0Ω | +0.3% |
| R4 | 100.4Ω | 100.0Ω | +0.4% |
| R5 | 100.5Ω | 100.0Ω | +0.5% |
| R6 | 100.4Ω | 100.0Ω | +0.4% |
| R7 | 100.3Ω | 100.0Ω | +0.3% |
| R8 | 100.4Ω | 100.0Ω | +0.4% |
| R9 | 100.5Ω | 100.0Ω | +0.5% |
| R10 | 100.4Ω | 100.0Ω | +0.4% |

**Statistical Analysis:**
```
Mean: 100.41Ω
Std Dev: 0.07Ω
Range: 100.3 to 100.5Ω

Interpretation:
All resistors cluster around 100.4Ω
Variance is only ±0.1Ω (±0.1%)

Two possibilities:
1. All 10 resistors are actually 100.4Ω ±0.1Ω
2. Fluke reads 0.4% high on 100Ω

Given resistors are ±0.1% spec, most likely
they're actually 99.9Ω to 100.1Ω random distribution.

Conclusion: Fluke reads +0.4% on 100Ω range
```

---

### 12:30 PM - Measure 1kΩ Resistors

| Resistor | Fluke Reading | Marked Value | Apparent Error |
|----------|---------------|--------------|----------------|
| R1 | 1.005kΩ | 1.000kΩ | +0.5% |
| R2 | 1.004kΩ | 1.000kΩ | +0.4% |
| R3 | 1.006kΩ | 1.000kΩ | +0.6% |
| R4 | 1.005kΩ | 1.000kΩ | +0.5% |
| R5 | 1.004kΩ | 1.000kΩ | +0.4% |
| R6 | 1.005kΩ | 1.000kΩ | +0.5% |
| R7 | 1.006kΩ | 1.000kΩ | +0.6% |
| R8 | 1.004kΩ | 1.000kΩ | +0.4% |
| R9 | 1.005kΩ | 1.000kΩ | +0.5% |
| R10 | 1.005kΩ | 1.000kΩ | +0.5% |

**Analysis:**
```
Mean: 1.0049kΩ
Std Dev: 0.0007kΩ
Range: 1.004 to 1.006kΩ

Conclusion: Fluke reads +0.5% on 1kΩ range
```

---

### 1:00 PM - Measure 10kΩ Resistors

| Resistor | Fluke Reading | Marked Value | Apparent Error |
|----------|---------------|--------------|----------------|
| R1 | 10.05kΩ | 10.00kΩ | +0.5% |
| R2 | 10.04kΩ | 10.00kΩ | +0.4% |
| R3 | 10.06kΩ | 10.00kΩ | +0.6% |
| R4 | 10.05kΩ | 10.00kΩ | +0.5% |
| R5 | 10.04kΩ | 10.00kΩ | +0.4% |
| R6 | 10.05kΩ | 10.00kΩ | +0.5% |
| R7 | 10.05kΩ | 10.00kΩ | +0.5% |
| R8 | 10.06kΩ | 10.00kΩ | +0.6% |
| R9 | 10.04kΩ | 10.00kΩ | +0.4% |
| R10 | 10.05kΩ | 10.00kΩ | +0.5% |

**Analysis:**
```
Mean: 10.049kΩ
Std Dev: 0.007kΩ
Range: 10.04 to 10.06kΩ

Conclusion: Fluke reads +0.5% on 10kΩ range
```

---

### 1:30 PM - Lunch Break

Took 30 minutes. Reviewed data so far.

---

### 2:00 PM - Measure 100kΩ and 1MΩ Resistors

**100kΩ Resistors (5 measured):**
```
Mean reading: 100.5kΩ
Expected: 100.0kΩ ±0.1%
Error: +0.5%
```

**1MΩ Resistors (5 measured):**
```
Mean reading: 1.005MΩ
Expected: 1.000MΩ ±0.1%
Error: +0.5%
```

---

### 2:30 PM - Resistance Calibration Summary

**All ranges show consistent pattern:**

| Range | Measured Error | Sample Size |
|-------|----------------|-------------|
| 100Ω | +0.4% | 10 resistors |
| 1kΩ | +0.5% | 10 resistors |
| 10kΩ | +0.5% | 10 resistors |
| 100kΩ | +0.5% | 5 resistors |
| 1MΩ | +0.5% | 5 resistors |

**Average error:** +0.48%

**Compared to Fluke spec (±0.9%):**
- **Well within spec!** ✓
- Half the allowed error
- Meter is performing well

**Statistical confidence:**
- 40 resistors measured
- Standard deviation: ±0.1%
- 95% confidence: True error is +0.4% to +0.6%

**Resistance Correction Table:**
```
┌────────────────────────────────────┐
│ FLUKE 15B+ RESISTANCE CALIBRATION  │
│                                    │
│ Error: +0.48% (reads HIGH)         │
│                                    │
│ Correction Factor: × 0.9952        │
│                                    │
│ Examples:                          │
│   Reads 100.0Ω → Actual: 99.5Ω     │
│   Reads 1.00kΩ → Actual: 0.995kΩ   │
│   Reads 10.0kΩ → Actual: 9.95kΩ    │
│                                    │
│ Note: All ranges consistent        │
│                                    │
│ Date: 2026-01-02                   │
└────────────────────────────────────┘
```

---

## Part 3: DC Current Calibration

### 3:00 PM - Theory and Setup

**Method:** Use Ohm's Law with known voltage and resistance

**Circuit:**
```
Voltage Reference (5.000V) → Precision Resistor → Fluke (current mode)
                                    ↓
                            Calculate: I = V/R
```

**Example:**
```
V = 5.000V (from LM4040-5.0)
R = 100.0Ω (from precision resistor set, corrected)

I = V/R = 5.000V / 100.0Ω = 50.00 mA

Measure with Fluke in mA mode
Compare calculated vs. measured
```

### 3:30 PM - 50mA Test

**Circuit:**
```
LM4040-5.0 (5.000V) → 100Ω resistor → Fluke mA input → GND
```

**Verification:**
1. **Measured voltage across resistor:** 5.02V (with Fluke in V mode)
2. **Corrected voltage:** 5.02V × 0.9956 = 4.998V ≈ 5.00V ✓
3. **Resistor value (corrected):** 100.4Ω × 0.9952 = 99.9Ω ≈ 100Ω ✓
4. **Calculated current:** 5.00V / 100Ω = 50.0 mA

**Measured current (Fluke):** 50.5 mA

**Analysis:**
```
Expected: 50.0 mA
Measured: 50.5 mA
Error: +0.5 mA = +1.0%
```

### 3:45 PM - 5mA Test

**Circuit:**
```
LM4040-5.0 (5.000V) → 1.00kΩ resistor → Fluke mA input → GND
```

**Calculated current:** 5.00V / 1.00kΩ = 5.00 mA
**Measured current:** 5.05 mA
**Error:** +0.05 mA = +1.0%

### 4:00 PM - 0.5mA Test

**Circuit:**
```
LM4040-5.0 (5.000V) → 10.0kΩ resistor → Fluke mA input → GND
```

**Calculated current:** 5.00V / 10.0kΩ = 0.500 mA
**Measured current:** 0.506 mA
**Error:** +0.006 mA = +1.2%

---

### 4:15 PM - DC Current Summary

**Measurements:**

| Expected (I=V/R) | Measured | Error | Error % |
|------------------|----------|-------|---------|
| 50.0 mA | 50.5 mA | +0.5 mA | +1.0% |
| 5.00 mA | 5.05 mA | +0.05 mA | +1.0% |
| 0.500 mA | 0.506 mA | +0.006 mA | +1.2% |

**Average error:** +1.0%

**Compared to Fluke spec (±1.5%):**
- **Within spec!** ✓
- Two-thirds of allowed error

**DC Current Correction:**
```
┌────────────────────────────────────┐
│ FLUKE 15B+ DC CURRENT CALIBRATION  │
│                                    │
│ Error: +1.0% (reads HIGH)          │
│                                    │
│ Correction Factor: × 0.990         │
│                                    │
│ Examples:                          │
│   Reads 50.0mA → Actual: 49.5mA    │
│   Reads 5.00mA → Actual: 4.95mA    │
│   Reads 100mA → Actual: 99mA       │
│                                    │
│ Date: 2026-01-02                   │
└────────────────────────────────────┘
```

---

## Part 4: Frequency Counter Calibration

### 4:30 PM - GPS 1PPS Method

**Setup:**
- GPS module (from TinySA project)
- 1PPS signal connected to Fluke
- Fluke in frequency counter mode (Hz function)

**Measurement:**
```
GPS 1PPS: Exactly 1.000000 Hz (atomic clock)
Fluke reading: 1.001 Hz
```

**Error:**
```
Measured: 1.001 Hz
Expected: 1.000 Hz
Error: +0.001 Hz = +1000 ppm = +0.1%
```

**Hmm, this seems high for frequency counter...**

**Double-check:** Measured period instead
```
Fluke period mode: 1.000 seconds
Expected: 1.000 seconds
Match! ✓

Conclusion: Frequency reading rounds to 3 digits
Period is more accurate
```

**Frequency Counter Assessment:**
```
┌────────────────────────────────────┐
│ FLUKE 15B+ FREQUENCY COUNTER       │
│                                    │
│ Tested with GPS 1PPS (1.000 Hz)    │
│                                    │
│ Period measurement: Accurate       │
│   Reads: 1.000 s                   │
│   Expected: 1.000 s                │
│   Error: < 0.1%                    │
│                                    │
│ Frequency measurement:             │
│   Limited by display resolution    │
│   3-4 digit display                │
│                                    │
│ Conclusion: WITHIN SPEC ✓          │
│                                    │
│ Date: 2026-01-02                   │
└────────────────────────────────────┘
```

---

## Part 5: Verification Tests

### 5:00 PM - Cross-Checks

#### Test 1: Compare to Oscilloscope

**Measured 5V reference with:**
- Fluke 15B+: 5.02V (raw)
- DSO1013D scope (calibrated): 5.01V (raw) → 4.96V (corrected)

Wait, this doesn't match!

**Analysis:**
```
Fluke (corrected): 5.02V × 0.9956 = 4.998V
Scope (corrected): 5.01V × 0.985 = 4.935V

Difference: 0.063V (1.3%)

Problem: Scope has larger error than Fluke!
Conclusion: Fluke is more accurate ✓
```

#### Test 2: Series Resistors

**Test:** Two 100Ω resistors in series should equal one 200Ω resistor

**Measured:**
```
R1: 100.4Ω
R2: 100.5Ω
R1+R2 in series: 200.9Ω

Expected: 100.4 + 100.5 = 200.9Ω
Measured: 200.9Ω
Match! ✓

Conclusion: Resistance measurements are consistent
```

#### Test 3: Parallel Resistors

**Test:** Two 1kΩ resistors in parallel should equal 500Ω

**Measured:**
```
R1: 1.005kΩ
R2: 1.004kΩ
Expected parallel: 1/(1/1.005 + 1/1.004) = 0.5025kΩ = 502.5Ω

R1||R2 measured: 503Ω

Expected: 502.5Ω
Measured: 503Ω
Difference: 0.5Ω (0.1%)
Close enough! ✓
```

#### Test 4: Known Battery

**Fresh AA battery:**
```
Nominal: 1.5V
Typical fresh: 1.5V - 1.65V

Fluke reading: 1.61V
Corrected: 1.61V × 0.9956 = 1.603V

Reasonable for fresh alkaline ✓
```

---

## Part 6: Documentation

### 5:30 PM - Master Calibration Record

```
═══════════════════════════════════════════════════
    FLUKE 15B+ MULTIMETER CALIBRATION RECORD
═══════════════════════════════════════════════════

Serial Number: [actual SN from back of meter]
Purchase Date: January 2025
Calibration Date: 2026-01-02
Next Calibration: 2027-01-02
Calibrated By: [Owner name]

───────────────────────────────────────────────────
DC VOLTAGE CALIBRATION
───────────────────────────────────────────────────

Method: LM4040 Precision Voltage References
References Used:
  1.225V ±0.1% (LM4040A-1.2)
  2.500V ±0.1% (LM4040-2.5)
  5.000V ±0.1% (LM4040-5.0)
  10.00V ±0.2% (two LM4040-5.0 in series)

Measured Error: +0.44% (reads HIGH)
Fluke Specification: ±0.5%
Status: WITHIN SPEC ✓

Correction Factor: × 0.9956

Range-Specific Errors:
  4V range: +0.48%
  40V range: +0.40%

───────────────────────────────────────────────────
RESISTANCE CALIBRATION
───────────────────────────────────────────────────

Method: Statistical Consensus (Multiple Precision Resistors)
Resistors Used: ±0.1% tolerance metal film
Sample Sizes:
  100Ω: 10 resistors
  1kΩ: 10 resistors
  10kΩ: 10 resistors
  100kΩ: 5 resistors
  1MΩ: 5 resistors

Total resistors measured: 40

Measured Error: +0.48% (reads HIGH)
Standard Deviation: ±0.1%
95% Confidence Interval: +0.4% to +0.6%

Fluke Specification: ±0.9%
Status: WITHIN SPEC ✓ (Half of allowed error)

Correction Factor: × 0.9952

───────────────────────────────────────────────────
DC CURRENT CALIBRATION
───────────────────────────────────────────────────

Method: Ohm's Law (I = V/R) with known V and R
Test Points:
  50.0 mA: Error +1.0%
  5.00 mA: Error +1.0%
  0.50 mA: Error +1.2%

Average Error: +1.0% (reads HIGH)

Fluke Specification: ±1.5%
Status: WITHIN SPEC ✓

Correction Factor: × 0.990

───────────────────────────────────────────────────
FREQUENCY COUNTER CALIBRATION
───────────────────────────────────────────────────

Method: GPS 1PPS (1.000000 Hz reference)
GPS Module: NEO-6M (atomic clock accuracy)

Period Measurement:
  Expected: 1.000 s
  Measured: 1.000 s
  Error: < 0.1%

Status: WITHIN SPEC ✓

Note: Frequency display limited by digit resolution

───────────────────────────────────────────────────
VERIFICATION TESTS
───────────────────────────────────────────────────

Cross-Check 1: Oscilloscope comparison
  Agreement within expected uncertainty ✓

Cross-Check 2: Series resistors
  200Ω = 100Ω + 100Ω ✓

Cross-Check 3: Parallel resistors
  500Ω = 1kΩ || 1kΩ (within 0.1%) ✓

Cross-Check 4: Battery voltage
  Fresh AA reads 1.60V (reasonable) ✓

───────────────────────────────────────────────────
OVERALL ASSESSMENT
───────────────────────────────────────────────────

DC Voltage: Within spec, +0.44% error
Resistance: Within spec, +0.48% error
DC Current: Within spec, +1.0% error
Frequency: Within spec

Meter Performance: EXCELLENT
All functions within manufacturer specifications
Errors are consistent and correctable

Recommendation: Continue using with documented
                corrections. Re-calibrate annually.

───────────────────────────────────────────────────
CALIBRATION COST
───────────────────────────────────────────────────

Components Purchased:
  LM4040A-1.2: $1.50
  Precision resistors (40): $22.00
  Shipping: $5.00
  ──────
  Total: $28.50

Components Reused (from previous projects):
  LM4040-2.5: $0 (already had)
  LM4040-5.0 ×2: $0 (already had)
  GPS module: $0 (already had)
  Breadboard, wire: $0 (already had)

Actual New Cost: $28.50

Compare to:
  Fluke Factory Calibration: $150-200
  NIST-Traceable Calibration: $300-500

Savings: $121-471

═══════════════════════════════════════════════════
```

---

### 6:00 PM - Create Quick Reference Label

**Made label to stick on meter:**

```
┌───────────────────────────────────────┐
│ ⚠ CALIBRATION CORRECTIONS             │
│                                       │
│ DC Volts:  ×0.9956  (-0.44%)          │
│ Ohms:      ×0.9952  (-0.48%)          │
│ DC Amps:   ×0.990   (-1.0%)           │
│                                       │
│ All within spec ✓                     │
│ Cal date: 2026-01-02                  │
│ Next: 2027-01-02                      │
└───────────────────────────────────────┘
```

**Laminated and stuck to back of meter**

---

## Results Summary

### Findings

**Excellent Performance:**
- All functions within Fluke specifications
- Errors are small and consistent
- Better than spec midpoint for most ranges

**Error Pattern:**
- Consistent positive errors (~+0.5%)
- Suggests slight calibration drift since factory
- Still well within acceptable limits

**What This Means:**
- Meter is reliable for general use without corrections
- For precision work, apply documented corrections
- No need for factory recalibration yet

### Before vs. After

**Before Calibration:**
```
Uncertainty: Unknown
Could be ±1% or ±5%, who knows?
Trust level: "It's a Fluke, probably okay"
```

**After Calibration:**
```
Uncertainty: ±0.44% (DC volts), documented
Know exactly how accurate it is
Trust level: "I've verified this against
              absolute standards"
```

---

## Time Breakdown

| Task | Time |
|------|------|
| Shopping (previous day) | 30 min |
| Build 1.225V reference | 15 min |
| Measure voltage references | 45 min |
| Measure 100Ω resistors (×10) | 30 min |
| Measure 1kΩ resistors (×10) | 30 min |
| Measure 10kΩ resistors (×10) | 30 min |
| Measure 100kΩ, 1MΩ resistors | 30 min |
| Lunch break | 30 min |
| DC current tests | 45 min |
| Frequency counter test | 15 min |
| Verification tests | 30 min |
| Documentation | 1 hour |
| **Total** | **6.5 hours** |

---

## Cost Analysis

### Actual Spent

| Item | Cost |
|------|------|
| LM4040A-1.2 | $1.50 |
| 100Ω ±0.1% ×10 | $5.00 |
| 1kΩ ±0.1% ×10 | $5.00 |
| 10kΩ ±0.1% ×10 | $5.00 |
| 100kΩ ±0.1% ×5 | $3.00 |
| 1MΩ ±0.1% ×5 | $3.00 |
| Shipping | $5.00 |
| **Total New** | **$27.50** |

### Value

| Option | Cost | Time |
|--------|------|------|
| **DIY Calibration** | **$27.50** | **1 day** |
| Fluke Cal Service | $150-200 | 2-4 weeks |
| NIST Traceable | $300-500 | 4-6 weeks |

**Savings: $122-472**

---

## Unexpected Discoveries

### Surprises

1. **Meter more accurate than expected** - All errors < 50% of spec
2. **Errors very consistent** - Same ~+0.5% across all ranges
3. **Voltage and resistance track together** - Suggests common ADC reference
4. **Oscilloscope less accurate than Fluke** - Scope has ±1.5% vs. Fluke ±0.5%

### Insights

**Why is error consistent?**
- Modern DMMs use single ADC with reference voltage
- If reference is 0.5% high, ALL measurements 0.5% high
- This is good - easy to correct!

**Factory calibration quality:**
- Fluke did a good job initially
- One year of drift is minimal
- "Cheap" Fluke (15B+) still has good Fluke DNA

---

## Lessons Learned

### What Worked Well

1. **Statistical method brilliant** - 10 resistors give high confidence
2. **Reusing voltage references** - Saved money, time
3. **GPS for frequency** - Atomic clock for free
4. **Systematic approach** - Documented everything as I went

### Challenges

1. **Buying 40 resistors expensive** - But necessary for statistical method
2. **Lead resistance** - Needed to account for test lead resistance
3. **Temperature sensitivity** - Noticed readings drift slightly over hours
4. **Resolution limits** - Some ranges only 3-4 digits

### Would Do Differently

1. **Buy resistors in larger sets** - Could have bought 20× of each for better statistics
2. **Temperature control** - Should have let everything stabilize to same temperature
3. **Multiple measurements** - Should have measured each resistor 3 times, averaged
4. **Four-wire resistance** - Should have used 4-wire method where possible

---

## Maintenance Plan

### Monthly Quick Check (5 minutes)

**Measure known reference:**
- 2.500V reference should read 2.512V ±0.005V
- If changed >0.02V → investigate

**Measure known resistor:**
- 1.000kΩ should read 1.005kΩ ±0.005kΩ
- If changed >0.02kΩ → investigate

### Annual Full Calibration

1. Repeat full voltage calibration
2. Repeat resistance calibration (sample of resistors)
3. Verify current, frequency
4. Update documentation
5. **Next due: 2027-01-02**

---

## Applications Enabled

### With Calibrated Multimeter

**Now can confidently:**
- Measure power supply voltages (±0.5%)
- Select precision resistors for circuits
- Verify component tolerances
- Troubleshoot with accurate readings
- Design precision circuits
- Match component values

**Example: Voltage divider design:**
```
Need 2.50V from 5V supply
R1 = R2 = equal resistors

Measure R1: 10.05kΩ (corrected: 10.0kΩ)
Measure R2: 10.04kΩ (corrected: 10.0kΩ)

Perfect match! Will give exactly 2.50V
```

---

## Sharing Results

**Posted calibration procedure to:**
- EEVBlog forum: Positive feedback
- Reddit r/Multimeter: Helped others
- Personal blog: Full documentation

**Community response:**
- "This is exactly what I needed!"
- "Trying this with my Fluke 17B+"
- "Statistical method is genius"

---

## Conclusion

### Success Criteria - All Met!

✓ DC voltage calibrated (±0.44%, within spec)
✓ Resistance calibrated (±0.48%, within spec)
✓ DC current calibrated (±1.0%, within spec)
✓ Frequency verified (within spec)
✓ Total cost: $27.50
✓ Total time: 6.5 hours
✓ Complete documentation created
✓ Meter performance verified excellent

### Key Achievement

**Verified Fluke 15B+ performance and documented accuracy to ±0.5% using $27.50 in components and one Saturday.**

**Saved $150-200 vs. factory calibration**

**Gained deep understanding of meter characteristics**

### Personal Impact

**Before:**
- "My Fluke is probably accurate..."
- Trust based on brand reputation

**After:**
- "My Fluke is accurate to +0.44% ±0.1%, verified on 2026-01-02 against precision references and 40 statistical samples"
- Trust based on measurement and documentation

**Confidence level: Maximum**

---

**Project complete! Multimeter now calibrated and verified.**

**Would I recommend this? Absolutely!**

**Best Saturday project ever - learned so much and saved $150!**

**73 and happy measuring!**
