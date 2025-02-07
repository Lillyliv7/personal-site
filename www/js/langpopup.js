function localizeUrl(url) {
    console.log("created url " + url.replace(/(\/[^\/]+)(\.\w+)$/, '$1-es$2') + " from " + url);
    return url.replace(/(\/[^\/]+)(\.\w+)$/, '$1-es$2');
}
function displayLangPopup() {
    const modal = document.getElementById('langPopup');
    modal.style.display = "flex";
}
function hideLangPopup() {
    const modal = document.getElementById('langPopup');
    modal.style.display = "none";
}
function langPopupCallback(answer) {
    if (answer)
        window.location.href = localizeUrl(window.location.href);
    else
        hideLangPopup();
}
if (navigator.language.startsWith('es') && !window.location.href.endsWith("-es.html")) {
    displayLangPopup();
}
