function checkWebsiteStatus() {
    const url = 'https://stats.weylyn.net'; // Replace 'https://example.com' with the website you want to check
    const statusElement = document.getElementById('status');
    const statusIconElement = document.getElementById('statusIcon');
    
    // Using a proxy to bypass CORS restrictions
    const proxyUrl = 'https://cors-anywhere.herokuapp.com/' + url;

    fetch(proxyUrl)
      .then(response => {
        if (response.ok) {
          statusElement.textContent = 'ON';
          statusIconElement.textContent = '✅'; 
        } else {
          statusElement.textContent = 'OFF';
          statusIconElement.textContent = '❌'; 
        }
      })
      .catch(() => {
        statusElement.textContent = 'OFF';
        statusIconElement.textContent = '❌'; 
      });
  }

  // Check website status initially
  checkWebsiteStatus();

  // Automatically check every 15 seconds
  setInterval(checkWebsiteStatus, 15000);