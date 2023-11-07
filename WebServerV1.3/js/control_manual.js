const themeToggler = document.querySelector(".theme-toggler");

themeToggler.addEventListener('click', ()=>{
    document.body.classList.toggle('dark-theme-variables');
    themeToggler.querySelector('span:nth-child(1)').classList.toggle('active');
    themeToggler.querySelector('span:nth-child(2)').classList.toggle('active');
})

const reiniciarFallasBtn = document.getElementById("reiniciarFallasBtn");
const reiniciarEnergiaBtn = document.getElementById("reiniciarEnergiaBtn");
const modoBypassCheckbox = document.getElementById("modoBypass");
const modoManualCheckbox = document.getElementById("modoManual");
const aa1Checkbox = document.getElementById("aa1");
const aa2Checkbox = document.getElementById("aa2");
const aa3Checkbox = document.getElementById("aa3");
const aa4Checkbox = document.getElementById("aa4");

function enviarEstadoAServidor(accion, valor) {
  const data = {
    accion: accion,
    valor: valor
  };

  fetch("/api/control_manual", {
    method: "POST",
    headers: {
      "Content-Type": "application/json"
    },
    body: JSON.stringify(data)
  })
  .then(response => {
    if (!response.ok) {
      throw new Error("Error al enviar el estado al servidor");
    }
    return response.json();
  })
  .then(responseData => {
    console.log("Respuesta del servidor:", responseData);
    actualizarEstadoDesdeJSON(responseData);
  })
  .catch(error => {
    console.error("Error:", error);
  });
}

// Agregar el evento click al botón "Reiniciar Contador de Fallas"
reiniciarFallasBtn.addEventListener("click", () => {
  // Lógica para cambiar el estado del botón (1 o 0)
  const nuevoValor = reiniciarFallasBtn.classList.contains("active") ? 0 : 1;
  reiniciarFallasBtn.classList.toggle("active", nuevoValor === 1);

  // Enviar el nuevo estado al servidor
  enviarEstadoAServidor("reiniciarFallas", nuevoValor);
});

// Agregar el evento click al botón "Reiniciar Contador de Energía"
reiniciarEnergiaBtn.addEventListener("click", () => {
  // Lógica para cambiar el estado del botón (1 o 0)
  const nuevoValor = reiniciarEnergiaBtn.classList.contains("active") ? 0 : 1;
  reiniciarEnergiaBtn.classList.toggle("active", nuevoValor === 1);

  // Enviar el nuevo estado al servidor
  enviarEstadoAServidor("reiniciarEnergia", nuevoValor);
});
modoBypassCheckbox.addEventListener("change", () => {
  const nuevoValor = modoBypassCheckbox.checked ? 1 : 0;
  enviarEstadoAServidor("modoBypass", nuevoValor);
});

// Agregar el evento change al switch "Modo Manual"
modoManualCheckbox.addEventListener("change", () => {
  const nuevoValor = modoManualCheckbox.checked ? 1 : 0;
  enviarEstadoAServidor("modoManual", nuevoValor);
});

// Agregar el evento change a los switches de los aires acondicionados
aa1Checkbox.addEventListener("change", () => {
  const nuevoValor = aa1Checkbox.checked ? 1 : 0;
  enviarEstadoAServidor("aa1", nuevoValor);
});

aa2Checkbox.addEventListener("change", () => {
  const nuevoValor = aa2Checkbox.checked ? 1 : 0;
  enviarEstadoAServidor("aa2", nuevoValor);
});

aa3Checkbox.addEventListener("change", () => {
  const nuevoValor = aa3Checkbox.checked ? 1 : 0;
  enviarEstadoAServidor("aa3", nuevoValor);
});

aa4Checkbox.addEventListener("change", () => {
  const nuevoValor = aa4Checkbox.checked ? 1 : 0;
  enviarEstadoAServidor("aa4", nuevoValor);
});

function actualizarEstadoDesdeJSON(data) {
  if (data.modoBypass !== undefined) {
      modoBypassCheckbox.checked = data.modoBypass === "1";
  }

  if (data.modoManual !== undefined) {
      modoManualCheckbox.checked = data.modoManual === "1";
  }

  if (data.aa1 !== undefined) {
      aa1Checkbox.checked = data.aa1 === "1";
  }

  if (data.aa2 !== undefined) {
      aa2Checkbox.checked = data.aa2 === "1";
  }

  if (data.aa3 !== undefined) {
      aa3Checkbox.checked = data.aa3 === "1";
  }

  if (data.aa4 !== undefined) {
      aa4Checkbox.checked = data.aa4 === "1";
  }
}

function obtenerYActualizarEstados() {
  fetch("/api/obtener_estados") // Cambia la URL a la que corresponda en tu servidor
      .then(response => response.json())
      .then(data => {
          actualizarEstadoDesdeJSON(data);
      })
      .catch(error => console.error(error));
}
obtenerYActualizarEstados();