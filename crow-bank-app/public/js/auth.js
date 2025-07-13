// Authentication management for SecureBank
class AuthManager {
  constructor() {
    this.isAuthenticated = false;
    this.currentUser = null;
    this.init();
  }

  init() {
    // Check if user is already logged in
    const token = localStorage.getItem("authToken");
    if (token) {
      this.isAuthenticated = true;
      // Set the token in the API instance
      if (window.api) {
        window.api.setToken(token);
      }
    }
  }

  async login(email, password) {
    try {
      const response = await window.api.login(email, password);

      if (response.success) {
        this.isAuthenticated = true;
        this.showMessage(
          "Login successful! Redirecting to dashboard...",
          "success"
        );

        // Redirect to dashboard after short delay
        setTimeout(() => {
          window.location.href = "/dashboard";
        }, 1500);

        return true;
      } else {
        this.showMessage(response.message || "Login failed", "error");
        return false;
      }
    } catch (error) {
      this.showMessage(error.message || "Login failed", "error");
      return false;
    }
  }

  async register(userData) {
    try {
      const response = await window.api.register(userData);

      if (response.success) {
        this.showMessage(
          "Registration successful! You can now login.",
          "success"
        );
        return true;
      } else {
        this.showMessage(response.message || "Registration failed", "error");
        return false;
      }
    } catch (error) {
      this.showMessage(error.message || "Registration failed", "error");
      return false;
    }
  }

  logout() {
    window.api.clearToken();
    this.isAuthenticated = false;
    this.currentUser = null;
    window.location.href = "/login";
  }

  showMessage(message, type = "info") {
    // Remove any existing messages
    const existingMessage = document.querySelector(".auth-message");
    if (existingMessage) {
      existingMessage.remove();
    }

    // Create new message element
    const messageDiv = document.createElement("div");
    messageDiv.className = `auth-message ${type}`;
    messageDiv.textContent = message;

    // Add styles
    messageDiv.style.cssText = `
            position: fixed;
            top: 20px;
            right: 20px;
            padding: 15px 20px;
            border-radius: 5px;
            color: white;
            font-weight: bold;
            z-index: 1000;
            animation: slideIn 0.3s ease-out;
        `;

    // Set background color based on type
    switch (type) {
      case "success":
        messageDiv.style.backgroundColor = "#28a745";
        break;
      case "error":
        messageDiv.style.backgroundColor = "#dc3545";
        break;
      case "warning":
        messageDiv.style.backgroundColor = "#ffc107";
        messageDiv.style.color = "#212529";
        break;
      default:
        messageDiv.style.backgroundColor = "#17a2b8";
    }

    // Add slide-in animation
    const style = document.createElement("style");
    style.textContent = `
            @keyframes slideIn {
                from { transform: translateX(100%); opacity: 0; }
                to { transform: translateX(0); opacity: 1; }
            }
        `;
    document.head.appendChild(style);

    // Add to document
    document.body.appendChild(messageDiv);

    // Auto-remove after 5 seconds
    setTimeout(() => {
      messageDiv.style.animation = "slideIn 0.3s ease-out reverse";
      setTimeout(() => {
        if (messageDiv.parentNode) {
          messageDiv.parentNode.removeChild(messageDiv);
        }
      }, 300);
    }, 5000);
  }

  // Check if user is authenticated (for protected routes)
  requireAuth() {
    const token = localStorage.getItem("authToken");
    if (!token || !this.isAuthenticated) {
      window.location.href = "/login";
      return false;
    }

    // Ensure API has the token
    if (window.api && !window.api.token) {
      window.api.setToken(token);
    }

    return true;
  }
}

// Create global auth manager instance
window.auth = new AuthManager();
