const themeToggler = document.querySelector(".theme-toggler");

themeToggler.addEventListener('click', ()=>{
    document.body.classList.toggle('dark-theme-variables');
    themeToggler.querySelector('span:nth-child(1)').classList.toggle('active');
    themeToggler.querySelector('span:nth-child(2)').classList.toggle('active');
})
function actualizarDatosMedidor() {

    fetch("/api/medidor") // Realiza una petición GET a la ruta /api/config_data en el servidor
        .then(response => response.json()) // Parsea la respuesta JSON
        .then(data => {
        // Actualiza los elementos HTML con los nuevos datos recibidos del servidor
        document.getElementById("VOLT").innerText = data.voltajeRMS + " V";
        document.getElementById("PA").innerText = data.potenciaActiva + " W";
        document.getElementById("PR").innerText = data.potenciaReactiva + " VAR";
        document.getElementById("PAP").innerText = data.potenciaAparente + " VA";
        document.getElementById("EA").innerText = data.energiaActiva + " kWh";
        document.getElementById("ER").innerText = data.energiaReactiva + " kVARh";
        document.getElementById("FP").innerText = data.factorPotencia;
        document.getElementById("FREC").innerText = data.frecuencia + " Hz";
        })
        .catch(error => console.error(error));
    }
    
actualizarDatosMedidor();
setInterval(actualizarDatosMedidor, 4000);