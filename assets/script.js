const hash = window.location.hash.substring(1);
const params = new URLSearchParams(hash);
const accessToken = params.get("access_token");
const statusEl = document.getElementById("status");
const loadingEl = document.getElementsByClassName("loading")[0];

if (accessToken) {
    fetch('/token?access_token=' + accessToken)
        .then(res => {
            if (res.ok) {
                statusEl.textContent = 'Authentication successful!';
                statusEl.classList.add('success');
                loadingEl.hidden = true;
                document.querySelector('p').textContent = 'You can close this tab now.';
            } else {
                throw new Error('Server error');
            }
        })
        .catch(() => {
            statusEl.textContent = 'Error saving token!';
            statusEl.classList.add('error');
            loadingEl.hidden = true;
            document.querySelector('p').textContent = 'Please try again.';
        });
} else {
    statusEl.textContent = 'No access token found!';
    statusEl.classList.add('error');
    loadingEl.hidden = true;
    document.querySelector('p').textContent = 'Authentication failed.';
}