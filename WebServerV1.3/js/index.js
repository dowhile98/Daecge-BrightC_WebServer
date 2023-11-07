const themeToggler = document.querySelector(".theme-toggler");


function showContent(option){
  document.getElementById("content").innerHTML = '';
  if (option === "estatus-actual") {
    document.getElementById("content").innerHTML = `
    <h1 id="ubicacion">-</h1>
    <div class="insights">
        <div class="air">
            <!-- <span class="material-symbols-sharp">heat_pump</span> -->
            <div class="middle">
                <div class="left">
                    <h3>Total de Aires Acondicionados</h3>
                    <h1 id="cantAA">-</h1>
                </div>
            </div>
        </div>
        <!-- END AA-->
        <div class="temp1">
            <!-- <span class="material-symbols-sharp">device_thermostat</span> -->
            <div class="middle">
                <div class="left">
                    <h3>Sensor de Temperatura 1</h3>
                    <h1 id="tempSensor1">- °C</h1>
                </div>
            </div>
        </div>
        <!--END CONFIG-->
        <div class="temp2">
            <!-- <span class="material-symbols-sharp">device_thermostat</span> -->
            <div class="middle">
                <div class="left">
                    <h3>Sensor de Temperatura 2</h3>
                    <h1 id="tempSensor2">- °C</h1>
                </div>
            </div>
        </div> 
        <!--end temp-->                
    </div>
    <!-- END OF ENSIGHTS-->
    <div class="actual-status" id="estado-aires-acondicionados">
        <h2>ESTADO ACTUAL DE LOS AIRES ACONDICIONADOS</h2>
        <table>
            <thead>
                <tr>
                    <th>Aire acondicionado</th>
                    <th>Turno</th>
                    <th>Estado</th>
                    <th>¿Ayuda?</th>
                    <th>Contador de fallas</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td><b>Aire Acondicionado 1</b></td>
                    <td id="turnoAA1">Activo</td>
                    <td id="estadoAA1">Apagado</td>
                    <td class="warning" id="helpAA1">NO</td>
                    <td class="primary" id="failCountAA1">0</td>
                </tr>
                <tr> 
                    <td><b>Aire Acondicionado 2</b></td>
                    <td id="turnoAA2">Activo</td>
                    <td id="estadoAA2">Apagado</td>
                    <td class="warning" id="helpAA2">NO</td>
                    <td class="primary" id="failCountAA2">0</td>
                </tr>
                <tr>
                    <td><b>Aire Acondicionado 3</b></td>
                    <td id="turnoAA3">Activo</td>
                    <td id="estadoAA3">Apagado</td>
                    <td class="warning" id="helpAA3">NO</td>
                    <td class="primary" id="failCountAA3">0</td>
                </tr>
                <tr>
                    <td><b>Aire Acondicionado 4</b></td>
                    <td id="turnoAA4">Activo</td>
                    <td id="estadoAA4">Apagado</td>
                    <td class="warning" id="helpAA4">NO</td>
                    <td class="primary" id="failCountAA4">0</td>
                </tr>
            </tbody>
        </table>
    </div>
    <!--ALARMAS-->
    <div class="alarms">
        <h2>ALARMAS</h2>
        <table>
            <thead>
                <tr>
                    <th>Nombre</th>
                    <th>Estado</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>BYPASS</td>
                    <td id="bypass"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td>Alarma de bajo voltaje AA2</td>
                    <td id="A_SAA1"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td >Alarma de bajo voltaje AA1</td>
                    <td id="A_ABAA1"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td>Alarma de suministro AA2</td>
                    <td id="A_SAA2"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td >Alarma de baja presión AA2</td>
                    <td id="A_ABAA2"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td>Alarma de alta presión AA2</td>
                    <td id="A_SAA3"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td >Alarma de suministro AA1</td>
                    <td id="A_ABAA3"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td>Alarma de baja presión AA1</td>
                    <td id="A_SAA4"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td >Alarma de alta presión AA1</td>
                    <td id="A_ABAA4"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td>Alarma de incendio</td>
                    <td id="AFIRE"><p><b>-</b></p></td>
                </tr>
                <tr>
                    <td>Alarma de alta Temperatura</td>
                    <td id="ATEMH"><p><b>-</b></p></td>
                </tr>
            </tbody>
        </table>
    </div>`;
    
  } else if (option === "configuracion") {
    document.getElementById("content").innerHTML =  `<h1 class="titulo">Configuración</h1>
    <form id="config-form">
        <div class="form-group">
            <label for="cantidad-aires">Cantidad de Aires Acondicionados:</label>
            <select id="cantidad-aires" name="cantidad-aires">
                <option value="1">1 AA</option>
                <option value="2">2 AA</option>
                <option value="3">3 AA</option>
                <option value="4">4 AA</option>
            </select>
        </div>
        <div class="form-group">
            <label for="dias-secuencia">Días de Secuencia:</label>
            <select id="dias-secuencia" name="dias-secuencia">
                <option value="7">7 días</option>
                <option value="15">15 días</option>
                <option value="20">20 días</option>
                <option value="30">30 días</option>
            </select>
        </div>
        <div class="form-group">
            <label for="tiempo-enfriar">Tiempo Máximo para Enfriar (minutos):</label>
            <input type="number" id="tiempo-enfriar" name="tiempo-enfriar" min="2" required="true">
        </div>
        <div class="form-group">
            <label for="temp-minima">Temperatura Mínima:</label>
            <input type="number" id="temp-minima" name="temp-minima" step="0.1" required="true">
        </div>
        <div class="form-group">
            <label for="temp-maxima">Temperatura Máxima:</label>
            <input type="number" id="temp-maxima" name="temp-maxima" step="0.1" required="true">
        </div>
        <div class="form-group">
            <label for="temp-alarma">Temperatura Alarma:</label>
            <input type="number" id="temp-alarma" name="temp-alarma" step="0.1" required="true">
        </div>
        <div class="form-group">
            <label for="temp-bypass">Temperatura Bypass:</label>
            <input type="number" id="temp-bypass" name="temp-bypass" step="0.1" required="true">
        </div>
        <button class="button" type="submit">Confirmar Configuración</button>
    </form>`;
  } else if (option === "control-manual") {
    document.getElementById("content").innerHTML =  `<h1 class="tituloCOntrol">Control Manual</h1>
    <div class="control-section">
        <h2>Reinicio de Contador de Fallas</h2>
        <button id="reiniciarFallasBtn">Reiniciar Contador</button>
    </div>
    <div class="control-section">
        <h2>Reinicio de Contador de Energía</h2>
        <button id="reiniciarEnergiaBtn">Reiniciar Contador</button>
    </div>
    <div class="control-section">
        <h2>Modo Bypass</h2>
        <label class="switch">
        <input type="checkbox" id="modoBypass">
        <span class="slider"></span>
        </label>
    </div>
    <div class="control-section">
        <h2>Modo Manual</h2>
        <label class="switch">
        <input type="checkbox" id="modoManual">
        <span class="slider"></span>
        </label>
        <div class="control-subsection" id="controlAAs">
            <div class="on-off-switch">
                <label for="aa1">AA1</label>
                <label class="switch">
                <input type="checkbox" id="aa1">
                <span class="slider"></span>
                </label>
            </div>
            <div class="on-off-switch">
                <label for="aa2">AA2</label>
                <label class="switch">
                <input type="checkbox" id="aa2">
                <span class="slider"></span>
                </label>
            </div>
            <div class="on-off-switch">
                <label for="aa3">AA3</label>
                <label class="switch">
                <input type="checkbox" id="aa3">
                <span class="slider"></span>
                </label>
            </div>
            <div class="on-off-switch">
                <label for="aa4">AA4</label>
                <label class="switch">
                <input type="checkbox" id="aa4">
                <span class="slider"></span>
                </label>
            </div>
        </div>
    </div>`;
  } else if (option === "medidor-electrico") {
    document.getElementById("content").innerHTML = `<h1>Medidor Eléctrico</h1>
    <table class="medidor-table">
        <thead>
            <tr>
                <th>Medida</th>
                <th>Valor de Medida</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>Voltaje RMS</td>
                <td id="VOLT">- V</td>
            </tr>
            <tr>
                <td>Potencia Activa</td>
                <td id="PA">- W</td>
            </tr>
            <tr>
                <td>Potencia Reactiva</td>
                <td id="PR">- VAR</td>
            </tr>
            <tr>
                <td>Potencia Aparente</td>
                <td id="PAP">- VA</td>
            </tr>
            <tr>
                <td>Energía Activa</td>
                <td id="EA">- kWh</td>
            </tr>
            <tr>
                <td>Energía Reactiva</td>
                <td id="ER">- kVARh</td>
            </tr>
            <tr>
                <td >Factor de Potencia</td>
                <td id="FP">-</td>
            </tr>
            <tr>
                <td>Frecuencia</td>
                <td id="FREC">- Hz</td>
            </tr>
        </tbody>
    </table>`;
  } else if (option === "update") {

  }
  
  const menuItems = document.querySelectorAll('.sidebar a');
  menuItems.forEach(item => item.classList.remove('selected'));
  document.querySelector(`.sidebar a[data-option="${option}"]`).classList.add('selected');
}

themeToggler.addEventListener('click', ()=>{
    document.body.classList.toggle('dark-theme-variables');
    themeToggler.querySelector('span:nth-child(1)').classList.toggle('active');
    themeToggler.querySelector('span:nth-child(2)').classList.toggle('active');
})

// function actualizarDatos() {
//     fetch("/api/estado_actual") // Realiza una petición GET a la ruta /api/estado_actual en el servidor
//       .then(response => response.json()) // Parsea la respuesta JSON
//       .then(data => {
//         // Actualiza los elementos HTML con los datos recibidos del servidor
//         document.getElementById("ubicacion").innerText = data.ubicacion;
//         document.getElementById("cantAA").innerText = data.cantAA;
//         document.getElementById("tempSensor1").innerText = data.tempSensor1 + " °C";
//         document.getElementById("tempSensor2").innerText = data.tempSensor2 + " °C";
//         // AA1
//         document.getElementById("turnoAA1").innerText = data.turno1;
//         document.getElementById("estadoAA1").innerText = data.estadoAA1;
//         document.getElementById("helpAA1").innerText = data.ayuda1;
//         document.getElementById("failCountAA1").innerText = data.contadorFallas1;
//         //AA2
//         document.getElementById("turnoAA2").innerText = data.turno2;
//         document.getElementById("estadoAA2").innerText = data.estadoAA2;
//         document.getElementById("helpAA2").innerText = data.ayuda2;
//         document.getElementById("failCountAA2").innerText = data.contadorFallas2;
//         //AA3
//         document.getElementById("turnoAA3").innerText = data.turno3;
//         document.getElementById("estadoAA3").innerText = data.estadoAA3;
//         document.getElementById("helpAA3").innerText = data.ayuda3;
//         document.getElementById("failCountAA3").innerText = data.contadorFallas3;
//         //AA4
//         document.getElementById("turnoAA4").innerText = data.turno4;
//         document.getElementById("estadoAA4").innerText = data.estadoAA4;
//         document.getElementById("helpAA4").innerText = data.ayuda4;
//         document.getElementById("failCountAA4").innerText = data.contadorFallas4;

//         //alarmas
//         document.getElementById("bypass").innerText = data.bypass;
//         document.getElementById("A_SAA1").innerText = data.A_SAA1;
//         document.getElementById("A_ABAA1").innerText = data.A_ABAA1;
//         document.getElementById("A_SAA2").innerText = data.A_SAA2;
//         document.getElementById("A_ABAA2").innerText = data.A_ABAA2;
//         document.getElementById("A_SAA3").innerText = data.A_SAA3;
//         document.getElementById("A_ABAA3").innerText = data.A_ABAA3;
//         document.getElementById("A_SAA4").innerText = data.A_SAA4;
//         document.getElementById("A_ABAA4").innerText = data.A_ABAA4;
//         document.getElementById("AFIRE").innerText = data.AFIRE;
//         document.getElementById("ATEMH").innerText = data.ATEMH;
//         // Aquí puedes continuar actualizando otros elementos de la página según los datos recibidos del servidor
//       })
//       .catch(error => console.error(error));
//   }

//   actualizarDatos();
//   setInterval(actualizarDatos, 5000); 

//   function updateConfigData() {
//     fetch("/api/config_data") // Realiza una petición GET a la ruta /api/config_data en el servidor
//       .then(response => response.json()) // Parsea la respuesta JSON
//       .then(data => {
//         // Actualiza los elementos HTML con los nuevos datos recibidos del servidor
//         document.getElementById("cantAAC").innerText = data.cantAAC;
//         document.getElementById("DSEC").innerText = data.DSEC + "dias";
//         document.getElementById("TMINC").innerText = data.TMINC + "°C";
//         document.getElementById("TMAXC").innerText = data.TMAXC + "°C";
//         document.getElementById("TALC").innerText = data.TALC + "°C";
//         document.getElementById("TBYC").innerText = data.TBYC + "°C";
//         document.getElementById("MINC").innerText = data.MINC + "min";
//       })
//       .catch(error => console.error(error));
//   }
//   updateConfigData();
//   setInterval(updateConfigData, 10000);
  