// turn a url like https://example.com/hello.html into https://example.com/hello-es.html
function localizeUrl(url) {
    console.log("created url " + url.replace(/(\/[^\/]+)(\.\w+)$/, '$1-es$2') + " from " + url);
    return url.replace(/(\/[^\/]+)(\.\w+)$/, '$1-es$2');
}
// create a popup to ask the user if they want to switch to the spanish version of the page
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
// ask the user if they would like to view the spanish version of a webpage if their browser language is in spanish and they are viewing an english version
if (navigator.language.startsWith('es') && !window.location.href.endsWith("-es.html")) {
    // const choice: boolean = window.confirm("Esta página está disponsible en español, ¿quieres ir allí ahora?");
    // if (choice)
    // window.location.href = localizeUrl(window.location.href);
    displayLangPopup();
}
