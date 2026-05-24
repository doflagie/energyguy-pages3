/*
 * smith_chart.h
 * Smith Chart drawing library for ILI9341 / TFT_eSPI
 *
 * Draws a full Smith chart with:
 *   - Outer unit circle
 *   - Resistance circles (r = 0, 0.5, 1, 2, 5)
 *   - Reactance arcs (x = ±0.5, ±1, ±2)
 *   - Center crosshairs
 *   - Impedance markers
 *   - Plotted measurement points with trail
 *
 * Coordinate system:
 *   Reflection coefficient Gamma = (Z - Z0) / (Z + Z0)
 *   Z0 = 50 ohm (normalized)
 *   Re(Gamma) = horizontal axis (left = -1, right = +1)
 *   Im(Gamma) = vertical axis (up = +1j = inductive, down = -1j = capacitive)
 *
 *   Smith chart convention: capacitive below center, inductive above center.
 *
 * Usage:
 *   SmithChart sc(tft, cx, cy, radius);
 *   sc.drawChart();
 *   sc.plotPoint(R_ohm, X_ohm, TFT_RED);
 *   sc.clearPoints();
 */

#pragma once
#include <TFT_eSPI.h>
#include <math.h>

#define SC_MAX_TRAIL 32

class SmithChart {
public:
    TFT_eSPI& _tft;
    int16_t _cx, _cy, _r;   // center x, center y, radius (pixels)

    // Point trail buffer
    struct Point { int16_t x, y; bool valid; };
    Point _trail[SC_MAX_TRAIL];
    int   _trail_head = 0;
    bool  _trail_full = false;

    // Colors
    uint16_t c_bg      = TFT_BLACK;
    uint16_t c_circle  = 0x2965;   // dark teal (resistance circles)
    uint16_t c_arc     = 0x0329;   // dark blue (reactance arcs)
    uint16_t c_axis    = 0x4208;   // dark gray (real axis)
    uint16_t c_outer   = TFT_WHITE;// outer unit circle
    uint16_t c_label   = 0x8410;   // gray labels
    uint16_t c_point   = TFT_RED;
    uint16_t c_trail   = 0xF800;   // dim red for trail

    SmithChart(TFT_eSPI& tft, int16_t cx, int16_t cy, int16_t r)
        : _tft(tft), _cx(cx), _cy(cy), _r(r) {
        for (int i = 0; i < SC_MAX_TRAIL; i++) _trail[i].valid = false;
    }

    // --------------------------------------------------------
    // Gamma to screen coordinates
    // --------------------------------------------------------
    void gammaToScreen(float re, float im, int16_t* sx, int16_t* sy) {
        *sx = (int16_t)(_cx + re * _r);
        *sy = (int16_t)(_cy - im * _r);  // Y inverted on screen
    }

    // --------------------------------------------------------
    // Z to Gamma
    // --------------------------------------------------------
    void zToGamma(float R, float X, float* re, float* im) {
        float Z0 = 50.0f;
        float Rn = R / Z0;
        float Xn = X / Z0;
        float denom = (Rn + 1.0f) * (Rn + 1.0f) + Xn * Xn;
        if (denom < 1e-6f) { *re = 1.0f; *im = 0.0f; return; }
        *re = (Rn * Rn + Xn * Xn - 1.0f) / denom;
        *im = (2.0f * Xn) / denom;
    }

    // --------------------------------------------------------
    // Draw a resistance circle (constant R)
    // Center: (R/(R+1), 0), Radius: 1/(R+1)
    // --------------------------------------------------------
    void drawResistanceCircle(float R_norm, uint16_t color) {
        float center_re = R_norm / (R_norm + 1.0f);
        float radius    = 1.0f / (R_norm + 1.0f);

        // Convert center and several points to screen
        // Draw as approximated polygon
        const int STEPS = 120;
        int16_t px = 0, py = 0, lx, ly;
        bool first = true;

        for (int i = 0; i <= STEPS; i++) {
            float angle = 2.0f * M_PI * i / STEPS;
            float re = center_re + radius * cosf(angle);
            float im = radius * sinf(angle);

            // Clip to unit circle
            if (re * re + im * im > 1.02f) continue;

            int16_t sx, sy;
            gammaToScreen(re, im, &sx, &sy);

            if (!first) {
                _tft.drawLine(lx, ly, sx, sy, color);
            }
            lx = sx; ly = sy;
            first = false;
        }
    }

    // --------------------------------------------------------
    // Draw a reactance arc (constant X)
    // Center of circle: (1, 1/X_norm), Radius: 1/|X_norm|
    // --------------------------------------------------------
    void drawReactanceArc(float X_norm, uint16_t color) {
        if (fabsf(X_norm) < 0.01f) return;

        float center_re = 1.0f;
        float center_im = 1.0f / X_norm;
        float radius    = 1.0f / fabsf(X_norm);

        const int STEPS = 200;
        int16_t lx = 0, ly = 0;
        bool first = true;

        for (int i = 0; i <= STEPS; i++) {
            float angle = 2.0f * M_PI * i / STEPS;
            float re = center_re + radius * cosf(angle);
            float im = center_im + radius * sinf(angle);

            // Clip to unit circle
            if (re * re + im * im > 1.01f) continue;
            // Clip to appropriate half-plane
            if (X_norm > 0 && im < -0.01f) continue;
            if (X_norm < 0 && im >  0.01f) continue;

            int16_t sx, sy;
            gammaToScreen(re, im, &sx, &sy);

            // Clip to visible area
            if (sx < _cx - _r - 2 || sx > _cx + _r + 2) { first=true; continue; }
            if (sy < _cy - _r - 2 || sy > _cy + _r + 2) { first=true; continue; }

            if (!first) {
                _tft.drawLine(lx, ly, sx, sy, color);
            }
            lx = sx; ly = sy;
            first = false;
        }
    }

    // --------------------------------------------------------
    // Draw the complete Smith chart background
    // --------------------------------------------------------
    void drawChart() {
        // Background circle (clear area)
        _tft.fillCircle(_cx, _cy, _r + 2, c_bg);

        // Outer unit circle
        _tft.drawCircle(_cx, _cy, _r, c_outer);

        // Real axis (horizontal center line)
        _tft.drawFastHLine(_cx - _r, _cy, _r * 2, c_axis);

        // Resistance circles: r = 0, 0.2, 0.5, 1, 2, 5, infinity(outer)
        drawResistanceCircle(0.0f, c_circle);   // r=0 (outer circle, re-draw brighter)
        drawResistanceCircle(0.2f, c_circle);
        drawResistanceCircle(0.5f, c_circle);
        drawResistanceCircle(1.0f, c_circle);   // center circle (at short/open midpoint)
        drawResistanceCircle(2.0f, c_circle);
        drawResistanceCircle(5.0f, c_circle);

        // Reactance arcs: x = ±0.2, ±0.5, ±1, ±2, ±5
        float xvals[] = {0.2f, 0.5f, 1.0f, 2.0f, 5.0f};
        for (int i = 0; i < 5; i++) {
            drawReactanceArc( xvals[i], c_arc);
            drawReactanceArc(-xvals[i], c_arc);
        }

        // Key reference labels (pixel-drawn)
        // Short (Gamma = -1, left): mark at _cx - _r
        _tft.fillCircle(_cx - _r, _cy, 3, c_label);
        // Open (Gamma = +1, right): mark at _cx + _r
        _tft.fillCircle(_cx + _r, _cy, 3, c_label);
        // 50 ohm (Gamma = 0, center): small crosshair
        _tft.drawFastHLine(_cx - 4, _cy, 8, c_label);
        _tft.drawFastVLine(_cx, _cy - 4, 8, c_label);

        // Outer ring annotations (tiny dots at key angles)
        // SWR circles would go here (optional)
    }

    // --------------------------------------------------------
    // Plot a measurement point (R, X in ohms)
    // --------------------------------------------------------
    void plotPoint(float R, float X, uint16_t color = 0) {
        if (color == 0) color = c_point;

        // Constrain R to positive
        if (R < 0.01f) R = 0.01f;

        float re, im;
        zToGamma(R, X, &re, &im);

        int16_t sx, sy;
        gammaToScreen(re, im, &sx, &sy);

        // Add to trail
        _trail[_trail_head] = {sx, sy, true};
        _trail_head = (_trail_head + 1) % SC_MAX_TRAIL;

        // Redraw trail (fading)
        for (int i = 0; i < SC_MAX_TRAIL; i++) {
            int idx = (_trail_head - i - 1 + SC_MAX_TRAIL) % SC_MAX_TRAIL;
            if (!_trail[idx].valid) continue;
            // Older points get dimmer
            uint16_t tcol = (i == 0) ? color :
                            (i < 4)  ? c_trail : c_circle;
            int r = (i == 0) ? 4 : (i < 4) ? 2 : 1;
            _tft.fillCircle(_trail[idx].x, _trail[idx].y, r, tcol);
        }
    }

    // --------------------------------------------------------
    // Clear all trail points and redraw chart
    // --------------------------------------------------------
    void clearPoints() {
        for (int i = 0; i < SC_MAX_TRAIL; i++) _trail[i].valid = false;
        _trail_head = 0;
        drawChart();
    }

    // --------------------------------------------------------
    // Draw SWR circle for a given SWR value
    // --------------------------------------------------------
    void drawSWRCircle(float swr, uint16_t color) {
        if (swr < 1.01f) return;
        float rho = (swr - 1.0f) / (swr + 1.0f);
        // SWR circle: center at (0,0) in Gamma plane, radius = rho
        int16_t r_pix = (int16_t)(rho * _r);
        if (r_pix > 0 && r_pix <= _r)
            _tft.drawCircle(_cx, _cy, r_pix, color);
    }

    // --------------------------------------------------------
    // Compute and return SWR from R, X
    // --------------------------------------------------------
    float computeSWR(float R, float X) {
        if (R < 0.01f) R = 0.01f;
        float re, im;
        zToGamma(R, X, &re, &im);
        float rho = sqrtf(re*re + im*im);
        rho = constrain(rho, 0.0f, 0.9999f);
        return (1.0f + rho) / (1.0f - rho);
    }

    // --------------------------------------------------------
    // Compute return loss in dB
    // --------------------------------------------------------
    float computeReturnLoss(float R, float X) {
        if (R < 0.01f) R = 0.01f;
        float re, im;
        zToGamma(R, X, &re, &im);
        float rho = sqrtf(re*re + im*im);
        if (rho < 1e-6f) return 99.0f;
        return -20.0f * log10f(rho);
    }
};
