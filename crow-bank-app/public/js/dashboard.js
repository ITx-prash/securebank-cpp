// Dashboard functionality for SecureBank
class Dashboard {
  constructor() {
    this.currentUser = null;
    this.transactionHistory = [];
    this.init();
  }

  async init() {
    // Ensure auth manager is available
    if (!window.auth) {
      console.error("Auth manager not available");
      window.location.href = "/login";
      return;
    }

    // Check authentication
    if (!window.auth.requireAuth()) {
      return;
    }

    try {
      // Load user data
      await this.loadUserData();
      this.setupEventListeners();
      this.showSection("balance");
    } catch (error) {
      console.error("Dashboard initialization failed:", error);
      window.auth.showMessage("Failed to load dashboard", "error");
      setTimeout(() => window.auth.logout(), 2000);
    }
  }

  async loadUserData() {
    try {
      console.log("Loading user data...");
      this.currentUser = await window.api.getUserData();
      console.log("User data loaded:", this.currentUser);
      this.updateUI();
    } catch (error) {
      console.error("Failed to load user data:", error);
      if (error.message && error.message.includes("Unauthorized")) {
        window.auth.showMessage(
          "Session expired. Please login again.",
          "error"
        );
        setTimeout(() => window.auth.logout(), 2000);
      } else {
        window.auth.showMessage(
          "Failed to load account data. Please try refreshing.",
          "error"
        );
      }
      throw error;
    }
  }

  updateUI() {
    if (!this.currentUser) return;

    // Update user info in navigation
    const userNameElement = document.getElementById("userName");
    if (userNameElement)
      userNameElement.textContent = `Welcome, ${this.currentUser.name}`;

    // Update balance in top cards
    const balanceElement = document.getElementById("balance");
    if (balanceElement)
      balanceElement.textContent = `$${this.currentUser.balance.toFixed(2)}`;

    const accountNumberElement = document.getElementById("accountNumber");
    if (accountNumberElement)
      accountNumberElement.textContent = this.currentUser.accountNumber;

    // Update detailed account info in balance section
    const accountHolderElement = document.getElementById("accountHolder");
    if (accountHolderElement)
      accountHolderElement.textContent = this.currentUser.name;

    const accountHolderDetailElement = document.getElementById(
      "accountHolderDetail"
    );
    if (accountHolderDetailElement)
      accountHolderDetailElement.textContent = this.currentUser.name;

    const accountNumberDetailElement = document.getElementById(
      "accountNumberDetail"
    );
    if (accountNumberDetailElement)
      accountNumberDetailElement.textContent = this.currentUser.accountNumber;

    const balanceDetailElement = document.getElementById("balanceDetail");
    if (balanceDetailElement)
      balanceDetailElement.textContent = `$${this.currentUser.balance.toFixed(
        2
      )}`;

    // Update Quick Stats
    this.updateQuickStats();
  }

  async updateQuickStats() {
    try {
      // Get transaction data to calculate stats
      const transactions = await window.api.getTransactions();
      let totalDeposits = 0;
      let totalWithdrawals = 0;
      let transactionCount = 0;

      // Calculate from detailed transactions if available
      if (
        transactions.detailedTransactions &&
        transactions.detailedTransactions.length > 0
      ) {
        transactions.detailedTransactions.forEach((trans) => {
          transactionCount++;
          if (trans.type === "Deposit") {
            totalDeposits += parseFloat(trans.amount);
          } else if (trans.type === "Withdrawal") {
            totalWithdrawals += parseFloat(trans.amount);
          }
        });
      }

      // Update Quick Stats display
      const totalDepositsElement = document.querySelector(
        ".text-emerald-400.font-medium"
      );
      if (totalDepositsElement) {
        totalDepositsElement.textContent = `$${totalDeposits.toFixed(2)}`;
      }

      const totalWithdrawalsElement = document.querySelector(
        ".text-red-400.font-medium"
      );
      if (totalWithdrawalsElement) {
        totalWithdrawalsElement.textContent = `$${totalWithdrawals.toFixed(2)}`;
      }

      const transactionCountElement = document.querySelector(
        ".text-blue-400.font-medium"
      );
      if (transactionCountElement) {
        transactionCountElement.textContent = transactionCount.toString();
      }
    } catch (error) {
      console.error("Failed to update quick stats:", error);
    }
  }

  setupEventListeners() {
    // Navigation buttons
    document.querySelectorAll(".nav-tab").forEach((btn) => {
      btn.addEventListener("click", (e) => {
        const section = e.target.dataset.section;
        this.showSection(section);
      });
    });

    // Logout button
    const logoutBtn = document.getElementById("logoutBtn");
    if (logoutBtn) {
      logoutBtn.addEventListener("click", () => window.auth.logout());
    }

    // Refresh button
    const refreshBtn = document.getElementById("refreshBtn");
    if (refreshBtn) {
      refreshBtn.addEventListener("click", async () => {
        refreshBtn.textContent = "Refreshing...";
        refreshBtn.disabled = true;
        try {
          await this.loadUserData();
          window.auth.showMessage(
            "Account data refreshed successfully",
            "success"
          );
        } catch (error) {
          window.auth.showMessage("Failed to refresh data", "error");
        } finally {
          refreshBtn.textContent = "Refresh Now";
          refreshBtn.disabled = false;
        }
      });
    }

    // Transaction forms
    this.setupTransactionForms();
  }

  setupTransactionForms() {
    // Deposit form
    const depositForm = document.getElementById("depositForm");
    if (depositForm) {
      depositForm.addEventListener("submit", async (e) => {
        e.preventDefault();
        const amount = parseFloat(
          document.getElementById("depositAmount").value
        );
        await this.handleDeposit(amount);
      });
    }

    // Withdraw form
    const withdrawForm = document.getElementById("withdrawForm");
    if (withdrawForm) {
      withdrawForm.addEventListener("submit", async (e) => {
        e.preventDefault();
        const amount = parseFloat(
          document.getElementById("withdrawAmount").value
        );
        await this.handleWithdraw(amount);
      });
    }

    // Transfer form
    const transferForm = document.getElementById("transferForm");
    if (transferForm) {
      transferForm.addEventListener("submit", async (e) => {
        e.preventDefault();
        const toEmail = document.getElementById("transferEmail").value;
        const amount = parseFloat(
          document.getElementById("transferAmount").value
        );
        await this.handleTransfer(toEmail, amount);
      });
    }
  }

  showSection(sectionName) {
    // Hide all sections
    document.querySelectorAll(".dashboard-section").forEach((section) => {
      section.classList.remove("active");
    });

    // Show selected section
    const targetSection = document.getElementById(sectionName + "Section");
    if (targetSection) {
      targetSection.classList.add("active");
    }

    // Update navigation
    document.querySelectorAll(".nav-tab").forEach((btn) => {
      btn.classList.remove("active");
    });

    const activeBtn = document.querySelector(`[data-section="${sectionName}"]`);
    if (activeBtn) {
      activeBtn.classList.add("active");
    }

    // Load section-specific data
    if (sectionName === "history") {
      this.loadTransactionHistory();
    }
  }

  async handleDeposit(amount) {
    if (!this.validateAmount(amount)) return;

    try {
      const response = await window.api.deposit(amount);

      if (response.success) {
        window.auth.showMessage(
          `Successfully deposited $${amount.toFixed(2)}`,
          "success"
        );
        this.currentUser.balance = response.newBalance;
        this.updateUI();
        document.getElementById("depositForm").reset();
      }
    } catch (error) {
      window.auth.showMessage(error.message || "Deposit failed", "error");
    }
  }

  async handleWithdraw(amount) {
    if (!this.validateAmount(amount)) return;

    try {
      const response = await window.api.withdraw(amount);

      if (response.success) {
        window.auth.showMessage(
          `Successfully withdrew $${amount.toFixed(2)}`,
          "success"
        );
        this.currentUser.balance = response.newBalance;
        this.updateUI();
        document.getElementById("withdrawForm").reset();
      }
    } catch (error) {
      window.auth.showMessage(error.message || "Withdrawal failed", "error");
    }
  }

  async handleTransfer(toEmail, amount) {
    if (!this.validateAmount(amount)) return;
    if (!this.validateEmail(toEmail)) return;

    try {
      const response = await window.api.transfer(toEmail, amount);

      if (response.success) {
        window.auth.showMessage(
          `Successfully transferred $${amount.toFixed(2)} to ${
            response.recipient
          }`,
          "success"
        );
        this.currentUser.balance = response.newBalance;
        this.updateUI();
        document.getElementById("transferForm").reset();
      }
    } catch (error) {
      window.auth.showMessage(error.message || "Transfer failed", "error");
    }
  }

  async loadTransactionHistory() {
    try {
      const transactions = await window.api.getTransactions();
      this.displayTransactionHistory(transactions);
    } catch (error) {
      window.auth.showMessage("Failed to load transaction history", "error");
    }
  }

  displayTransactionHistory(transactions) {
    const historyContainer = document.getElementById("transactionHistory");
    if (!historyContainer) return;

    let html = "";

    // Display account transactions
    if (
      transactions.accountTransactions &&
      transactions.accountTransactions.length > 0
    ) {
      transactions.accountTransactions.forEach((trans) => {
        html += `<div class="transaction-item">${trans}</div>`;
      });
    }

    // Display detailed transactions
    if (
      transactions.detailedTransactions &&
      transactions.detailedTransactions.length > 0
    ) {
      html +=
        '<div class="transaction-group"><h4>Detailed Transaction Log</h4>';
      transactions.detailedTransactions.forEach((trans) => {
        html += `
                    <div class="transaction-detail">
                        <strong>ID:</strong> ${trans.id} | 
                        <strong>Type:</strong> ${trans.type} | 
                        <strong>Amount:</strong> $${trans.amount.toFixed(2)} | 
                        <strong>Time:</strong> ${trans.timestamp}
                    </div>
                `;
      });
      html += "</div>";
    }

    if (
      transactions.accountTransactions.length === 0 &&
      transactions.detailedTransactions.length === 0
    ) {
      html += "<p>No transactions found.</p>";
    }

    historyContainer.innerHTML = html;
  }

  validateAmount(amount) {
    if (isNaN(amount) || amount <= 0) {
      window.auth.showMessage("Please enter a valid amount", "warning");
      return false;
    }
    return true;
  }

  validateEmail(email) {
    const emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/;
    if (!emailRegex.test(email)) {
      window.auth.showMessage("Please enter a valid email address", "warning");
      return false;
    }
    return true;
  }
}

// Initialize dashboard when DOM is loaded
document.addEventListener("DOMContentLoaded", () => {
  window.dashboard = new Dashboard();
});
