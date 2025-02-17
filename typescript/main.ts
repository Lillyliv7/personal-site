function newTab(url: string){
    window.open(url, '_blank').focus();
}


const socket = new WebSocket("wss://imlillith888.xyz:3000", "stats-protocol");

socket.onopen = () => {
    console.log("Connected to WebSocket server with protocol:", socket.protocol);
    socket.send("hello");
};

socket.onmessage = (event) => {
    console.log("Received:", event.data);
};

socket.onerror = (error) => {
    console.error("WebSocket error:", error);
};

socket.onclose = () => {
    console.log("WebSocket connection closed");
};
