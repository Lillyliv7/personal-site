function localizeUrl(url: string, lang: string): string {
    // Remove a trailing "-xx" language code (if any) that immediately precedes ".html"
    var urlWithoutLang = url.replace(/(-[a-z]{2})(?=\.html$)/i, '');
    
    // For English, return the URL without any language code.
    if (lang === 'en') {
        return urlWithoutLang;
    }
    
    // For other languages, insert the language code before ".html"
    return urlWithoutLang.replace(/\.html$/, '-' + lang + '.html');
}

// create a popup to ask the user if they want to switch to the spanish version of the page
function displayLangPopup() {
    const modal = document.getElementById('langPopup') as HTMLElement;
    modal.style.display = "flex";
}

function hideLangPopup() {
    const modal = document.getElementById('langPopup') as HTMLElement;
    modal.style.display = "none";
}

function langPopupCallback(answer: boolean) {
    if (!window.location.href.includes("es.html")) {
        if(answer) window.location.href = localizeUrl(window.location.href, 'es');
        else hideLangPopup();
    } else {
        if(answer) window.location.href = localizeUrl(window.location.href, 'en');
        else hideLangPopup();
    }
}
if ((navigator.language.startsWith('es') && !window.location.href.endsWith("-es.html")) || (navigator.language.startsWith('en') && window.location.href.endsWith("-es.html"))) {
    displayLangPopup();
}
