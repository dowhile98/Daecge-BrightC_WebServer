const modoManualCheckbox = document.getElementById("modoManual");
const onOffControl = document.getElementById("onOffControl");
const reiniciarFallasBtn = document.getElementById("reiniciarFallasBtn");
const reiniciarEnergiaBtn = document.getElementById("reiniciarEnergiaBtn");

modoManualCheckbox.addEventListener("change", () => {
  if (modoManualCheckbox.checked) {
    onOffControl.classList.add("show");
  } else {
    onOffControl.classList.remove("show");
  }
});

reiniciarFallasBtn.addEventListener("click", (event) => {
  event.preventDefault();
  // Lógica para reiniciar contador de fallas
});

reiniciarEnergiaBtn.addEventListener("click", (event) => {
  event.preventDefault();
  // Lógica para reiniciar contador de energía
});
