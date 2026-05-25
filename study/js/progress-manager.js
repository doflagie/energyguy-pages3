/**
 * Progress Manager for Ham Radio Flash Card Quiz
 *
 * Handles localStorage persistence for:
 * - Quiz sessions and scores
 * - User settings
 * - Question history
 *
 * @author Mervyn Martin / EnergyGuy LLC
 * @date January 2026
 */

class ProgressManager {
    constructor() {
        this.storageKey = 'hamRadioQuizProgress';
        this.data = this.load();
    }

    /**
     * Load data from localStorage
     */
    load() {
        try {
            const stored = localStorage.getItem(this.storageKey);
            if (stored) {
                return JSON.parse(stored);
            }
        } catch (e) {
            console.warn('Failed to load progress from localStorage:', e);
        }

        // Default structure
        return {
            sessions: [],
            settings: {
                audioEnabled: true,
                timerMode: 'study',
                questionSet: 'technician',
                showExplanations: true
            },
            bookmarks: [],
            lastAccessed: new Date().toISOString()
        };
    }

    /**
     * Save data to localStorage
     */
    save() {
        try {
            this.data.lastAccessed = new Date().toISOString();
            localStorage.setItem(this.storageKey, JSON.stringify(this.data));
            return true;
        } catch (e) {
            console.error('Failed to save progress to localStorage:', e);
            return false;
        }
    }

    /**
     * Start a new quiz session
     */
    startSession(questionSet, mode, questionCount) {
        const session = {
            id: `session-${Date.now()}`,
            startTime: new Date().toISOString(),
            endTime: null,
            questionSet: questionSet,
            mode: mode,
            totalQuestions: questionCount,
            correct: 0,
            incorrect: 0,
            score: 0,
            timeSeconds: 0,
            answers: {},
            completed: false
        };

        this.currentSession = session;
        return session;
    }

    /**
     * Record an answer for the current session
     */
    recordAnswer(questionId, selectedLetter, isCorrect, correctLetter) {
        if (!this.currentSession) return;

        this.currentSession.answers[questionId] = {
            selected: selectedLetter,
            correct: isCorrect,
            correctAnswer: correctLetter,
            timestamp: new Date().toISOString()
        };

        if (isCorrect) {
            this.currentSession.correct++;
        } else {
            this.currentSession.incorrect++;
        }

        this.save();
    }

    /**
     * End the current session
     */
    endSession(timeSeconds) {
        if (!this.currentSession) return;

        this.currentSession.endTime = new Date().toISOString();
        this.currentSession.timeSeconds = timeSeconds;
        this.currentSession.completed = true;

        // Calculate final score
        const total = this.currentSession.totalQuestions;
        const correct = this.currentSession.correct;
        this.currentSession.score = total > 0 ? Math.round((correct / total) * 100) : 0;

        // Save to sessions history
        this.data.sessions.push(this.currentSession);

        // Keep only last 50 sessions
        if (this.data.sessions.length > 50) {
            this.data.sessions = this.data.sessions.slice(-50);
        }

        this.save();

        const result = { ...this.currentSession };
        this.currentSession = null;
        return result;
    }

    /**
     * Get current session
     */
    getCurrentSession() {
        return this.currentSession;
    }

    /**
     * Get session history
     */
    getSessionHistory(limit = 10) {
        return this.data.sessions.slice(-limit).reverse();
    }

    /**
     * Get statistics across all sessions
     */
    getStatistics() {
        const sessions = this.data.sessions.filter(s => s.completed);

        if (sessions.length === 0) {
            return {
                totalSessions: 0,
                totalQuestions: 0,
                totalCorrect: 0,
                averageScore: 0,
                bestScore: 0,
                worstScore: 0
            };
        }

        const totalQuestions = sessions.reduce((sum, s) => sum + s.totalQuestions, 0);
        const totalCorrect = sessions.reduce((sum, s) => sum + s.correct, 0);
        const scores = sessions.map(s => s.score);

        return {
            totalSessions: sessions.length,
            totalQuestions: totalQuestions,
            totalCorrect: totalCorrect,
            averageScore: Math.round(scores.reduce((a, b) => a + b, 0) / scores.length),
            bestScore: Math.max(...scores),
            worstScore: Math.min(...scores)
        };
    }

    /**
     * Get/set user settings
     */
    getSetting(key) {
        return this.data.settings[key];
    }

    setSetting(key, value) {
        this.data.settings[key] = value;
        this.save();
    }

    getSettings() {
        return { ...this.data.settings };
    }

    updateSettings(settings) {
        this.data.settings = { ...this.data.settings, ...settings };
        this.save();
    }

    /**
     * Bookmark management
     */
    addBookmark(questionId) {
        if (!this.data.bookmarks.includes(questionId)) {
            this.data.bookmarks.push(questionId);
            this.save();
        }
    }

    removeBookmark(questionId) {
        const index = this.data.bookmarks.indexOf(questionId);
        if (index > -1) {
            this.data.bookmarks.splice(index, 1);
            this.save();
        }
    }

    isBookmarked(questionId) {
        return this.data.bookmarks.includes(questionId);
    }

    getBookmarks() {
        return [...this.data.bookmarks];
    }

    /**
     * Clear all data
     */
    clearAll() {
        if (confirm('Clear all quiz progress and settings? This cannot be undone.')) {
            localStorage.removeItem(this.storageKey);
            this.data = this.load();
            return true;
        }
        return false;
    }

    /**
     * Export data as JSON
     */
    exportData() {
        const dataStr = JSON.stringify(this.data, null, 2);
        const blob = new Blob([dataStr], { type: 'application/json' });
        const url = URL.createObjectURL(blob);
        const a = document.createElement('a');
        a.href = url;
        a.download = `ham-radio-quiz-progress-${new Date().toISOString().split('T')[0]}.json`;
        a.click();
        URL.revokeObjectURL(url);
    }
}

// Export for use in other modules
if (typeof module !== 'undefined' && module.exports) {
    module.exports = ProgressManager;
}
