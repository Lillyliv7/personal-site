if (!window.location.href.endsWith(".html")) {
    console.log("reformatting url to include index.html at the end");
    window.location.href = window.location.href + "index.html";
}
