// Utility functions for SecureBank
class Utils {
  // Format currency with proper decimal places
  static formatCurrency(amount) {
    return new Intl.NumberFormat("en-US", {
      style: "currency",
      currency: "USD",
      minimumFractionDigits: 2,
      maximumFractionDigits: 2,
    }).format(amount);
  }

  // Format date for display
  static formatDate(dateString) {
    const date = new Date(dateString);
    return date.toLocaleDateString("en-US", {
      year: "numeric",
      month: "short",
      day: "numeric",
      hour: "2-digit",
      minute: "2-digit",
    });
  }

  // Validate email format
  static validateEmail(email) {
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    return emailRegex.test(email);
  }

  // Validate phone number (basic validation)
  static validatePhone(phone) {
    const phoneRegex = /^[\+]?[1-9][\d]{0,15}$/;
    return phoneRegex.test(phone.replace(/[\s\-\(\)]/g, ""));
  }

  // Validate password strength
  static validatePassword(password) {
    const minLength = 6;
    const hasUpperCase = /[A-Z]/.test(password);
    const hasLowerCase = /[a-z]/.test(password);
    const hasNumbers = /\d/.test(password);

    return {
      isValid: password.length >= minLength,
      length: password.length >= minLength,
      hasUpperCase,
      hasLowerCase,
      hasNumbers,
      score: [
        password.length >= minLength,
        hasUpperCase,
        hasLowerCase,
        hasNumbers,
      ].filter(Boolean).length,
    };
  }

  // Sanitize input to prevent XSS
  static sanitizeInput(input) {
    const div = document.createElement("div");
    div.textContent = input;
    return div.innerHTML;
  }

  // Debounce function for search/input delays
  static debounce(func, wait) {
    let timeout;
    return function executedFunction(...args) {
      const later = () => {
        clearTimeout(timeout);
        func(...args);
      };
      clearTimeout(timeout);
      timeout = setTimeout(later, wait);
    };
  }

  // Copy text to clipboard
  static async copyToClipboard(text) {
    try {
      await navigator.clipboard.writeText(text);
      return true;
    } catch (err) {
      // Fallback for older browsers
      const textArea = document.createElement("textarea");
      textArea.value = text;
      document.body.appendChild(textArea);
      textArea.select();
      try {
        document.execCommand("copy");
        document.body.removeChild(textArea);
        return true;
      } catch (err) {
        document.body.removeChild(textArea);
        return false;
      }
    }
  }

  // Show loading spinner
  static showLoader(elementId) {
    const element = document.getElementById(elementId);
    if (element) {
      element.innerHTML = `
                <div class="loader">
                    <div class="spinner"></div>
                    <span>Loading...</span>
                </div>
            `;
    }
  }

  // Hide loading spinner and restore content
  static hideLoader(elementId, content = "") {
    const element = document.getElementById(elementId);
    if (element) {
      element.innerHTML = content;
    }
  }

  // Generate random transaction ID for client-side tracking
  static generateTempId() {
    return "temp_" + Date.now() + "_" + Math.random().toString(36).substr(2, 9);
  }

  // Check if device is mobile
  static isMobile() {
    return /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(
      navigator.userAgent
    );
  }

  // Local storage helpers with error handling
  static setStorage(key, value) {
    try {
      localStorage.setItem(key, JSON.stringify(value));
      return true;
    } catch (e) {
      console.error("Storage error:", e);
      return false;
    }
  }

  static getStorage(key, defaultValue = null) {
    try {
      const item = localStorage.getItem(key);
      return item ? JSON.parse(item) : defaultValue;
    } catch (e) {
      console.error("Storage error:", e);
      return defaultValue;
    }
  }

  static removeStorage(key) {
    try {
      localStorage.removeItem(key);
      return true;
    } catch (e) {
      console.error("Storage error:", e);
      return false;
    }
  }

  // Form validation helper
  static validateForm(formData, rules) {
    const errors = {};

    for (const [field, rule] of Object.entries(rules)) {
      const value = formData[field];

      if (rule.required && (!value || value.trim() === "")) {
        errors[field] = `${rule.label || field} is required`;
        continue;
      }

      if (value && rule.minLength && value.length < rule.minLength) {
        errors[field] = `${rule.label || field} must be at least ${
          rule.minLength
        } characters`;
        continue;
      }

      if (value && rule.maxLength && value.length > rule.maxLength) {
        errors[field] = `${rule.label || field} must be no more than ${
          rule.maxLength
        } characters`;
        continue;
      }

      if (value && rule.pattern && !rule.pattern.test(value)) {
        errors[field] = rule.message || `Invalid ${rule.label || field} format`;
        continue;
      }

      if (value && rule.custom && !rule.custom(value)) {
        errors[field] = rule.message || `Invalid ${rule.label || field}`;
      }
    }

    return {
      isValid: Object.keys(errors).length === 0,
      errors,
    };
  }

  // Display form errors
  static displayFormErrors(errors) {
    // Clear existing errors
    document.querySelectorAll(".error-message").forEach((el) => el.remove());
    document.querySelectorAll(".form-field.error").forEach((el) => {
      el.classList.remove("error");
    });

    // Display new errors
    for (const [field, message] of Object.entries(errors)) {
      const fieldElement =
        document.getElementById(field) ||
        document.querySelector(`[name="${field}"]`);
      if (fieldElement) {
        // Add error class to field
        const fieldContainer =
          fieldElement.closest(".form-field") || fieldElement.parentElement;
        fieldContainer.classList.add("error");

        // Create error message element
        const errorElement = document.createElement("div");
        errorElement.className = "error-message";
        errorElement.textContent = message;
        errorElement.style.cssText = `
                    color: #dc3545;
                    font-size: 0.875rem;
                    margin-top: 5px;
                `;

        // Insert error message after field
        fieldContainer.appendChild(errorElement);
      }
    }
  }
}

// Create global utils instance
window.utils = Utils;
