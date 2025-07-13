// API utility functions for SecureBank
class API {
  constructor() {
    this.baseURL = window.location.origin;
    this.token = localStorage.getItem("authToken");
  }

  // Set authentication token
  setToken(token) {
    this.token = token;
    localStorage.setItem("authToken", token);
  }

  // Clear authentication token
  clearToken() {
    this.token = null;
    localStorage.removeItem("authToken");
  }

  // Get headers with authentication
  getHeaders(includeAuth = false) {
    const headers = {
      "Content-Type": "application/json",
    };

    if (includeAuth && this.token) {
      headers["Authorization"] = `Bearer ${this.token}`;
    }

    return headers;
  }

  // Generic API request method
  async request(endpoint, options = {}) {
    const url = `${this.baseURL}${endpoint}`;
    const config = {
      ...options,
      headers: {
        ...this.getHeaders(options.auth),
        ...options.headers,
      },
    };

    try {
      const response = await fetch(url, config);
      const data = await response.json();

      if (!response.ok) {
        throw new Error(data.message || data.error || "Request failed");
      }

      return data;
    } catch (error) {
      console.error("API Request Error:", error);
      throw error;
    }
  }

  // Authentication methods
  async login(email, password) {
    const data = await this.request("/api/login", {
      method: "POST",
      body: JSON.stringify({ email, password }),
    });

    if (data.success) {
      this.setToken(data.token);
    }

    return data;
  }

  async register(userData) {
    return await this.request("/api/register", {
      method: "POST",
      body: JSON.stringify(userData),
    });
  }

  // User data methods
  async getUserData() {
    return await this.request("/api/user-data", {
      method: "GET",
      auth: true,
    });
  }

  // Banking operations
  async deposit(amount) {
    return await this.request("/api/deposit", {
      method: "POST",
      body: JSON.stringify({ amount }),
      auth: true,
    });
  }

  async withdraw(amount) {
    return await this.request("/api/withdraw", {
      method: "POST",
      body: JSON.stringify({ amount }),
      auth: true,
    });
  }

  async transfer(toEmail, amount) {
    return await this.request("/api/transfer", {
      method: "POST",
      body: JSON.stringify({ toEmail, amount }),
      auth: true,
    });
  }

  async getTransactions() {
    return await this.request("/api/transactions", {
      method: "GET",
      auth: true,
    });
  }
}

// Create global API instance
window.api = new API();
