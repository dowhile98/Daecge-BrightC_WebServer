const themeToggler = document.querySelector(".theme-toggler");

themeToggler.addEventListener('click', ()=>{
    document.body.classList.toggle('dark-theme-variables');
    themeToggler.querySelector('span:nth-child(1)').classList.toggle('active');
    themeToggler.querySelector('span:nth-child(2)').classList.toggle('active');
})

const configForm = document.getElementById("config-form");

configForm.addEventListener('submit', event => {
    event.preventDefault();

    const formData = new FormData(configForm);
    const data = Object.fromEntries(formData);

    fetch('/api/configuracion', {
        method : 'POST',
        headers : {
            'Content-Type' : 'application/json'
        },
        body : JSON.stringify(data)
    }) .then( res => res.json())
        .then(data => {
            alert("Configuración establecida!");
            console.log(data);
        })
        .catch(error => {
            console.log(error);
            alert("Falló la configuración");
        })
});

function updateConfigData() {
    fetch("/api/config_data") // Realiza una petición GET a la ruta /api/config_data en el servidor
      .then(response => response.json()) // Parsea la respuesta JSON
      .then(data => {
        // Actualiza los elementos HTML con los nuevos datos recibidos del servidor
        document.getElementById("cantAAC").innerText = data.cantAAC;
        document.getElementById("DSEC").innerText = data.DSEC + "dias";
        document.getElementById("TMINC").innerText = data.TMINC + "°C";
        document.getElementById("TMAXC").innerText = data.TMAXC + "°C";
        document.getElementById("TALC").innerText = data.TALC + "°C";
        document.getElementById("TBYC").innerText = data.TBYC + "°C";
        document.getElementById("MINC").innerText = data.MINC + "min";
      })
      .catch(error => console.error(error));
  }
  updateConfigData();
  setInterval(updateConfigData, 10000);