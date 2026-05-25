/**
 * Audio Manager for Ham Radio Flash Card Quiz
 *
 * Generates audio feedback using Web Audio API
 * - Correct answer: Pleasant 800Hz tone
 * - Incorrect answer: Harsh buzzer (300Hz + 600Hz)
 * - Quiz complete: Ascending tone sequence
 *
 * @author Mervyn Martin / EnergyGuy LLC
 * @date January 2026
 */

class AudioManager {
    constructor() {
        this.audioContext = null;
        this.enabled = true;
        this.initialized = false;
    }

    /**
     * Initialize audio context (must be called after user interaction)
     */
    init() {
        if (this.initialized) return;

        try {
            this.audioContext = new (window.AudioContext || window.webkitAudioContext)();
            this.initialized = true;
            console.log('AudioManager initialized');
        } catch (e) {
            console.warn('Web Audio API not supported:', e);
            this.enabled = false;
        }
    }

    /**
     * Play correct answer sound (pleasant tone)
     */
    playCorrect() {
        if (!this.enabled || !this.initialized) return;

        const now = this.audioContext.currentTime;

        // Create oscillator (800Hz sine wave)
        const osc = this.audioContext.createOscillator();
        const gain = this.audioContext.createGain();

        osc.connect(gain);
        gain.connect(this.audioContext.destination);

        osc.frequency.value = 800;
        osc.type = 'sine';

        // Envelope: quick attack, exponential decay
        gain.gain.setValueAtTime(0, now);
        gain.gain.linearRampToValueAtTime(0.3, now + 0.01);
        gain.gain.exponentialRampToValueAtTime(0.01, now + 0.15);

        osc.start(now);
        osc.stop(now + 0.15);
    }

    /**
     * Play incorrect answer sound (harsh buzzer)
     */
    playIncorrect() {
        if (!this.enabled || !this.initialized) return;

        const now = this.audioContext.currentTime;

        // Create two oscillators for dissonant buzzer
        const osc1 = this.audioContext.createOscillator();
        const osc2 = this.audioContext.createOscillator();
        const gain = this.audioContext.createGain();

        osc1.connect(gain);
        osc2.connect(gain);
        gain.connect(this.audioContext.destination);

        osc1.frequency.value = 300;
        osc2.frequency.value = 600;
        osc1.type = 'sawtooth';
        osc2.type = 'square';

        // Envelope: harsh attack, sustain
        gain.gain.setValueAtTime(0, now);
        gain.gain.linearRampToValueAtTime(0.4, now + 0.02);
        gain.gain.exponentialRampToValueAtTime(0.01, now + 0.2);

        osc1.start(now);
        osc2.start(now);
        osc1.stop(now + 0.2);
        osc2.stop(now + 0.2);
    }

    /**
     * Play quiz complete sound (ascending tones)
     */
    playComplete() {
        if (!this.enabled || !this.initialized) return;

        const now = this.audioContext.currentTime;
        const frequencies = [523, 659, 784, 1047]; // C, E, G, C (C major chord)
        const duration = 0.15;

        frequencies.forEach((freq, i) => {
            const startTime = now + (i * duration);
            const osc = this.audioContext.createOscillator();
            const gain = this.audioContext.createGain();

            osc.connect(gain);
            gain.connect(this.audioContext.destination);

            osc.frequency.value = freq;
            osc.type = 'sine';

            gain.gain.setValueAtTime(0, startTime);
            gain.gain.linearRampToValueAtTime(0.2, startTime + 0.01);
            gain.gain.exponentialRampToValueAtTime(0.01, startTime + duration);

            osc.start(startTime);
            osc.stop(startTime + duration);
        });
    }

    /**
     * Toggle audio on/off
     */
    toggle() {
        this.enabled = !this.enabled;
        return this.enabled;
    }

    /**
     * Set audio enabled state
     */
    setEnabled(enabled) {
        this.enabled = enabled;
    }

    /**
     * Get current enabled state
     */
    isEnabled() {
        return this.enabled;
    }
}

// Export for use in other modules
if (typeof module !== 'undefined' && module.exports) {
    module.exports = AudioManager;
}
