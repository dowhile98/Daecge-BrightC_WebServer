const themeToggler = document.querySelector(".theme-toggler");

themeToggler.addEventListener('click', ()=>{
    document.body.classList.toggle('dark-theme-variables');
    themeToggler.querySelector('span:nth-child(1)').classList.toggle('active');
    themeToggler.querySelector('span:nth-child(2)').classList.toggle('active');
})


const menuLinks = document.querySelectorAll(".sidebar a");

menuLinks.forEach(link => {
    link.addEventListener("click", (event) => {
        event.preventDefault();
        
        // Eliminamos la clase 'active' de todos los enlaces del menú
        menuLinks.forEach(link => link.classList.remove("active"));
        
        // Agregamos la clase 'active' solo al enlace que se ha hecho clic
        link.classList.add("active");

        // Cargar el contenido del menú correspondiente al enlace en el contenedor 'content'
        const menuContentURL = link.getAttribute("href");

        if (menuContentURL === "./status_actual.html") {
          actualizarDatos();
        } else if (menuContentURL === "./configuracion.html") {
            updateConfigData();
        } else if (menuContentURL === "./medidor_electrico.html") {
            actualizarDatosMedidor();
        }
        fetch(menuContentURL)
            .then(response => response.text())
            .then(htmlContent => {
                document.getElementById("content").innerHTML = htmlContent;
            })
            .catch(error => console.error(error));
    });
});

// Cargar el menú "Estatus actual" por defecto al cargar la página
const defaultMenu = document.querySelector(".sidebar a[href='./status_actual.html']");
defaultMenu.classList.add("active");
const defaultMenuContentURL = defaultMenu.getAttribute("href");
fetch(defaultMenuContentURL)
    .then(response => response.text())
    .then(htmlContent => {
        document.getElementById("content").innerHTML = htmlContent;
    })
    .catch(error => console.error(error));


// status_actual.js

// Función para obtener y actualizar los datos de estado actual desde el servidor
function actualizarDatos() {
    fetch("/api/estado_actual") // Realiza una petición GET a la ruta /api/estado_actual en el servidor
      .then(response => response.json()) // Parsea la respuesta JSON
      .then(data => {
        // Actualiza los elementos HTML con los datos recibidos del servidor
        document.getElementById("cantAA").innerText = data.cantAA;
        document.getElementById("tempSensor1").innerText = data.tempSensor1 + " °C";
        document.getElementById("tempSensor2").innerText = data.tempSensor2 + " °C";
        // AA1
        document.getElementById("turnoAA1").innerText = data.turno1;
        document.getElementById("estadoAA1").innerText = data.estadoAA1;
        document.getElementById("helpAA1").innerText = data.ayuda1;
        document.getElementById("failCountAA1").innerText = data.contadorFallas1;
        //AA2
        document.getElementById("turnoAA2").innerText = data.turno2;
        document.getElementById("estadoAA2").innerText = data.estadoAA2;
        document.getElementById("helpAA2").innerText = data.ayuda2;
        document.getElementById("failCountAA2").innerText = data.contadorFallas2;
        //AA3
        document.getElementById("turnoAA3").innerText = data.turno3;
        document.getElementById("estadoAA3").innerText = data.estadoAA3;
        document.getElementById("helpAA3").innerText = data.ayuda3;
        document.getElementById("failCountAA3").innerText = data.contadorFallas3;
        //AA4
        document.getElementById("turnoAA4").innerText = data.turno4;
        document.getElementById("estadoAA4").innerText = data.estadoAA4;
        document.getElementById("helpAA4").innerText = data.ayuda4;
        document.getElementById("failCountAA4").innerText = data.contadorFallas4;

        //alarmas
        document.getElementById("bypass").innerText = data.bypass;
        document.getElementById("A_SAA1").innerText = data.A_SAA1;
        document.getElementById("A_ABAA1").innerText = data.A_ABAA1;
        document.getElementById("A_SAA2").innerText = data.A_SAA2;
        document.getElementById("A_ABAA2").innerText = data.A_ABAA2;
        document.getElementById("A_SAA3").innerText = data.A_SAA3;
        document.getElementById("A_ABAA3").innerText = data.A_ABAA3;
        document.getElementById("A_SAA4").innerText = data.A_SAA4;
        document.getElementById("A_ABAA4").innerText = data.A_ABAA4;
        document.getElementById("AFIRE").innerText = data.AFIRE;
        document.getElementById("ATEMH").innerText = data.ATEMH;
        // Aquí puedes continuar actualizando otros elementos de la página según los datos recibidos del servidor
      })
      .catch(error => console.error(error));
  }
  
  // Función para actualizar los datos cada cierto intervalo (por ejemplo, cada 5 segundos)
  function actualizarDatosPeriodicamente() {
    setInterval(actualizarDatos, 5000); // Actualiza los datos cada 5 segundos (ajusta el valor según tus necesidades)
  }
  actualizarDatosPeriodicamente();
  // Llama a la función para actualizar los datos por primera vez al cargar la página
  //actualizarDatos();


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
  setInterval(updateConfigData, 120000);

  // Función para actualizar los datos del medidor eléctrico
function actualizarDatosMedidor() {

  fetch("/api/medidor") // Realiza una petición GET a la ruta /api/config_data en el servidor
      .then(response => response.json()) // Parsea la respuesta JSON
      .then(data => {
      // Actualiza los elementos HTML con los nuevos datos recibidos del servidor
      document.getElementById("VOLT").innerText = data.voltajeRMS;
      document.getElementById("PA").innerText = data.potenciaActiva;
      document.getElementById("PR").innerText = data.potenciaReactiva;
      document.getElementById("PAP").innerText = data.potenciaAparente;
      document.getElementById("EA").innerText = data.energiaActiva;
      document.getElementById("ER").innerText = data.energiaReactiva;
      document.getElementById("FP").innerText = data.factorPotencia;
      document.getElementById("FREC").innerText = data.frecuencia;
      })
      .catch(error => console.error(error));
  }
  
  setInterval(actualizarDatosMedidor, 4000);

// ... Resto del código en index.js ...

