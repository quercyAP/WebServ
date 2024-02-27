$(document).ready(() => {
  $(".button-collapse").sideNav();
});

const html = document.querySelector("html");

html.addEventListener("mousemove", (e) => {
    const x = e.clientX / window.innerWidth;
    const y = e.clientY / window.innerHeight;
    const gradient = `linear-gradient(${45 + x * 180}deg, #031a32, #0b3a50)`;
    html.style.background = gradient;
});

const toggleMenu = document.querySelector("#toggle");

const container = document.querySelector(".container");

const redirCard = document.querySelector(".redir");
const cgiCard = document.querySelector(".cgi");
const otherCard = document.querySelectorAll(".other");

const allTab = document.querySelector(".tab-all");

const redirTab = document.querySelector(".tab-redir");
const cgiTab = document.querySelector(".tab-cgi");
const otherTab = document.querySelector(".tab-other");

redirTab.addEventListener("click", () => {
    redirCard.style.display = "";
    cgiCard.style.display = "none";
    otherCard.forEach((card) => {
        card.style.display = "none";
    });
});

cgiTab.addEventListener("click", () => {
    redirCard.style.display = "none";
    cgiCard.style.display = "";
    otherCard.forEach((card) => {
      card.style.display = "none";
  });
});

otherTab.addEventListener("click", () => {
    redirCard.style.display = "none";
    cgiCard.style.display = "none";
    otherCard.forEach((card) => {
        card.style.display = "";
    });
});

allTab.addEventListener("click", () => {
    redirCard.style.display = "";
    cgiCard.style.display = "";
    otherCard.forEach((card) => {
        card.style.display = "";
    });
});

const submitBtn = document.querySelector("#submit");

submitBtn.addEventListener("click", envoyerFichier);

function envoyerFichier() {
    const fileInput = document.querySelector("#fileInput");
    const file = fileInput.files[0];
  
    if (file) {
      const xhr = new XMLHttpRequest();
      xhr.open("POST", "/upload", true);
      xhr.setRequestHeader("Content-Type", file.type);
      xhr.setRequestHeader("Filename", file.name);
      
      xhr.onloadstart = function () {
        console.log("Envoi du fichier en cours...");
      };
      
      xhr.onload = function () {
        if (xhr.status === 201) {
          // Traitement réussi
          console.log("Fichier envoyé avec succès.");
        } else {
          // Gérer les erreurs ici
          console.error("Une erreur est survenue lors de l'envoi du fichier.");
        }
      };
  
      xhr.send(file);
    } else {
      console.error("Aucun fichier sélectionné.");
    }
};

const form = document.querySelector(".formul");

form.addEventListener("submit", (e) => {
  e.preventDefault();
});
  
const deleteBtn = document.querySelector("#delete");

deleteBtn.addEventListener("click", deleteFile);

function deleteFile()
{
    const fileInput = document.getElementById("deleted");
    fetch(fileInput.value, {
        method: 'DELETE',
    })
};

const deleted = document.querySelector(".deleted");

deleted.addEventListener("click", (e) => {
  e.preventDefault();
});

const downloadBtn = document.querySelector("#download");

downloadBtn.addEventListener("click", downloadFile);

function downloadFile() {
    const fileInput = document.getElementById("downloaded");

    window.location.href = fileInput.value;
};

const downloaded = document.querySelector(".downloaded");

downloaded.addEventListener("click", (e) => {
  e.preventDefault();
});