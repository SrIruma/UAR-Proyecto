#include "color.h"

namespace color {
	std::string applyColor(const std::string& text, const std::string& color, bool bold) {
		return (bold ? BOLD : "") + color + text + RESET;
	}

	std::string white(const std::string& text, bool bold) {
		return applyColor(text, WHITE, bold);
	}

	std::string yellow(const std::string& text, bool bold) {
		return applyColor(text, YELLOW, bold);
	}

	std::string green(const std::string& text, bool bold) {
		return applyColor(text, GREEN, bold);
	}

	std::string red(const std::string& text, bool bold) {
		return applyColor(text, RED, bold);
	}

	std::string cyan(const std::string& text, bool bold) {
		return applyColor(text, CYAN, bold);
	}

	std::string blue(const std::string& text, bool bold) {
		return applyColor(text, BLUE, bold);
	}

	std::string magenta(const std::string& text, bool bold) {
		return applyColor(text, MAGENTA, bold);
	}

	std::string black(const std::string& text, bool bold) {
		return applyColor(text, BLACK, bold);
	}

	std::string brightRed(const std::string& text, bool bold) {
		return applyColor(text, BRIGHT_RED, bold);
	}

	std::string brightGreen(const std::string& text, bool bold) {
		return applyColor(text, BRIGHT_GREEN, bold);
	}

	std::string brightYellow(const std::string& text, bool bold) {
		return applyColor(text, BRIGHT_YELLOW, bold);
	}

	std::string brightBlue(const std::string& text, bool bold) {
		return applyColor(text, BRIGHT_BLUE, bold);
	}

	std::string brightMagenta(const std::string& text, bool bold) {
		return applyColor(text, BRIGHT_MAGENTA, bold);
	}

	std::string brightCyan(const std::string& text, bool bold) {
		return applyColor(text, BRIGHT_CYAN, bold);
	}
}
