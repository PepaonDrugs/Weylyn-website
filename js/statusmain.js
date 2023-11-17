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
          statusElement.textContent = 'Server Offline, Proxy or Server error occurred';
          statusIconElement.textContent = '❌'; 
        }
      })
      .catch(() => {
        statusElement.textContent = 'Server Offline, Proxy or Server error occurred';
        statusIconElement.textContent = '❌'; 
      });
  }

  // Check website status initially
  checkWebsiteStatus();

