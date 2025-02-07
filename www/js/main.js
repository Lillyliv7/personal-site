// Get the browser's language setting
const userLanguage = navigator.language;
console.log(userLanguage);
console.log(window.location.href.split('/')[window.location.href.split('/').length - 1]);
if (userLanguage.startsWith('es')) {
    // ask the user if they would like to view the spanish version
    const choice = window.confirm("Esta página está disponsible en español, ¿quieres ir allí ahora?");
    if (choice)
        window.location.href = 'index-es.html';
}
