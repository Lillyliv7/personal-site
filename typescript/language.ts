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

function determineCurrentLanguage(): string {
    const LANG_ES = 'es';
    const LANG_EN = 'en';
    
    // 3. More reliable language detection using URL path
    const currentPath = window.location.pathname;
    return currentPath.includes('es.html') ? LANG_ES : LANG_EN;
}

function swapLanguageString(lang: string): string {
    return lang === "en" ? "es" : "en";
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

function langPopupCallback(userWantsRedirect: boolean) {
    // 1. Handle checkbox safely and add type guard
    const box = document.getElementById('rememberDecision');
    if (!(box instanceof HTMLInputElement)) {
        console.error('Language preference checkbox not found');
        return;
    }

    // 2. Use constants for maintainability
    const LANG_ES = 'es';
    const LANG_EN = 'en';
    
    // 3. More reliable language detection using URL path
    const currentPath = window.location.pathname;
    const currentLang = currentPath.includes('es.html') ? LANG_ES : LANG_EN;
    
    // 4. Determine target language based on user choice
    const targetLang = userWantsRedirect 
        ? (currentLang === LANG_ES ? LANG_EN : LANG_ES)
        : currentLang;

    // 5. Clear or set preference before any potential redirect
    if (box.checked) {
        localStorage.setItem("languagePref", targetLang);
    } else {
        localStorage.removeItem("languagePref");
    }

    // 6. Execute appropriate action based on user choice
    if (userWantsRedirect) {
        // 7. Validate URL before redirecting
        const newUrl = localizeUrl(window.location.href, targetLang);
        if (newUrl && newUrl !== window.location.href) {
            window.location.href = newUrl;
        }
    } else {
        // 8. Always hide popup when declining redirect
        hideLangPopup();
    }
}

function languageButton() {
    localStorage.removeItem("languagePref");
    window.location.href = localizeUrl(window.location.href, swapLanguageString(determineCurrentLanguage()));
}

// check if we need to redirect automatically
if (localStorage.getItem("languagePref") != determineCurrentLanguage() && localStorage.getItem("languagePref") != null) {
    window.location.href = localizeUrl(window.location.href,swapLanguageString(determineCurrentLanguage()));
}

// check if we need to display language popup
if (
    ((navigator.language.startsWith('es') && !window.location.href.endsWith("-es.html")) 
    || (navigator.language.startsWith('en') && window.location.href.endsWith("-es.html")))
    && (localStorage.getItem('languagePref') == null)
) {
    displayLangPopup();
}
