/**
 * Quiz Engine for Ham Radio Flash Card Quiz
 *
 * Core logic for question management, answer validation, and scoring
 *
 * @author Mervyn Martin / EnergyGuy LLC
 * @date January 2026
 */

class QuizEngine {
    constructor(questionPool, settings = {}) {
        this.allQuestions = questionPool.questions || [];
        this.questionSet = questionPool.questionSet || 'technician';
        this.questions = [];
        this.currentIndex = 0;
        this.settings = {
            count: settings.count || 35,
            shuffle: settings.shuffle !== false,
            shuffleAnswers: settings.shuffleAnswers !== false,
            ...settings
        };
    }

    /**
     * Initialize quiz with selected questions
     */
    init() {
        // Select questions
        if (this.settings.count >= this.allQuestions.length) {
            this.questions = [...this.allQuestions];
        } else {
            // Randomly select subset
            this.questions = this.shuffleArray([...this.allQuestions])
                .slice(0, this.settings.count);
        }

        // Shuffle questions if enabled
        if (this.settings.shuffle) {
            this.questions = this.shuffleArray(this.questions);
        }

        // Prepare each question
        this.questions.forEach(q => {
            q.shuffledAnswers = this.settings.shuffleAnswers
                ? this.shuffleArray([...q.answers])
                : [...q.answers];
            q.userAnswer = null;
            q.isCorrect = null;
        });

        this.currentIndex = 0;
    }

    /**
     * Get current question
     */
    getCurrentQuestion() {
        if (this.currentIndex >= this.questions.length) {
            return null;
        }

        const question = this.questions[this.currentIndex];
        return {
            ...question,
            number: this.currentIndex + 1,
            total: this.questions.length,
            answers: question.shuffledAnswers
        };
    }

    /**
     * Select an answer for current question
     */
    selectAnswer(letter) {
        if (this.currentIndex >= this.questions.length) {
            return null;
        }

        const question = this.questions[this.currentIndex];

        // Find the selected answer
        const selectedAnswer = question.shuffledAnswers.find(a => a.letter === letter);
        if (!selectedAnswer) {
            return null;
        }

        // Find the correct answer
        const correctAnswer = question.answers.find(a => a.correct);

        // Record answer
        question.userAnswer = letter;
        question.isCorrect = selectedAnswer.correct;

        return {
            correct: selectedAnswer.correct,
            correctLetter: correctAnswer.letter,
            explanation: question.explanation || 'No explanation available.'
        };
    }

    /**
     * Move to next question
     */
    nextQuestion() {
        if (this.currentIndex < this.questions.length - 1) {
            this.currentIndex++;
            return true;
        }
        return false;
    }

    /**
     * Move to previous question
     */
    previousQuestion() {
        if (this.currentIndex > 0) {
            this.currentIndex--;
            return true;
        }
        return false;
    }

    /**
     * Jump to specific question
     */
    goToQuestion(index) {
        if (index >= 0 && index < this.questions.length) {
            this.currentIndex = index;
            return true;
        }
        return false;
    }

    /**
     * Get current progress
     */
    getProgress() {
        const answered = this.questions.filter(q => q.userAnswer !== null).length;
        const correct = this.questions.filter(q => q.isCorrect === true).length;
        const incorrect = this.questions.filter(q => q.isCorrect === false).length;

        return {
            current: this.currentIndex + 1,
            total: this.questions.length,
            answered: answered,
            unanswered: this.questions.length - answered,
            correct: correct,
            incorrect: incorrect,
            percentage: answered > 0 ? Math.round((correct / answered) * 100) : 0,
            isComplete: answered === this.questions.length
        };
    }

    /**
     * Get final score and results
     */
    getResults() {
        const progress = this.getProgress();
        const total = this.questions.length;
        const correct = progress.correct;
        const score = Math.round((correct / total) * 100);
        const passing = score >= 74; // FCC Technician passing score

        // Calculate by section
        const sections = {};
        this.questions.forEach(q => {
            const section = q.section || 'General';
            if (!sections[section]) {
                sections[section] = { total: 0, correct: 0 };
            }
            sections[section].total++;
            if (q.isCorrect) {
                sections[section].correct++;
            }
        });

        // Convert to array with percentages
        const sectionResults = Object.entries(sections).map(([name, data]) => ({
            name: name,
            total: data.total,
            correct: data.correct,
            percentage: Math.round((data.correct / data.total) * 100)
        }));

        return {
            total: total,
            correct: correct,
            incorrect: progress.incorrect,
            score: score,
            passing: passing,
            sections: sectionResults,
            questions: this.questions.map((q, i) => ({
                number: i + 1,
                id: q.id,
                text: q.text,
                userAnswer: q.userAnswer,
                correctAnswer: q.answers.find(a => a.correct).letter,
                isCorrect: q.isCorrect,
                section: q.section
            }))
        };
    }

    /**
     * Check if quiz is complete
     */
    isComplete() {
        return this.questions.every(q => q.userAnswer !== null);
    }

    /**
     * Get list of all questions (for navigation)
     */
    getAllQuestions() {
        return this.questions.map((q, i) => ({
            index: i,
            number: i + 1,
            id: q.id,
            answered: q.userAnswer !== null,
            correct: q.isCorrect
        }));
    }

    /**
     * Shuffle array using Fisher-Yates algorithm
     */
    shuffleArray(array) {
        const shuffled = [...array];
        for (let i = shuffled.length - 1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i + 1));
            [shuffled[i], shuffled[j]] = [shuffled[j], shuffled[i]];
        }
        return shuffled;
    }

    /**
     * Reset quiz
     */
    reset() {
        this.questions.forEach(q => {
            q.userAnswer = null;
            q.isCorrect = null;
        });
        this.currentIndex = 0;
    }

    /**
     * Get current question index
     */
    getCurrentIndex() {
        return this.currentIndex;
    }

    /**
     * Get total question count
     */
    getTotalQuestions() {
        return this.questions.length;
    }
}

// Export for use in other modules
if (typeof module !== 'undefined' && module.exports) {
    module.exports = QuizEngine;
}
