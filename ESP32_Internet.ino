// ==========================================================
//                    ESP32 + IPv6 INTERNET
// ==========================================================
//
// Ce code montre comment accéder à un ESP32
// directement depuis Internet grâce à l'IPv6.
//
// Fonctionnalités :
//
// ✔ Connexion WiFi
// ✔ Activation IPv6
// ✔ Serveur Web
// ✔ Accès depuis Internet
// ✔ Login / mot de passe
// ✔ Affichage IPv4 et IPv6
//
// Test :
//
// 1. Téléverser le code
// 2. Ouvrir le moniteur série
// 3. Récupérer l'adresse IPv6
// 4. Depuis téléphone en 5G :
//
//    http://[IPv6]
//
// ==========================================================

#include <WiFi.h>
#include <WebServer.h>

// ==========================================================
//                  PARAMETRES WIFI
// ==========================================================

// Nom du WiFi
const char* ssid = "TON_WIFI";

// Mot de passe WiFi
const char* password = "TON_MOT_DE_PASSE";

// ==========================================================
//               LOGIN / MOT DE PASSE WEB
// ==========================================================

// Login demandé dans le navigateur
const char* www_user = "admin";

// Mot de passe demandé dans le navigateur
const char* www_pass = "1234";

// ==========================================================
//                  SERVEUR WEB
// ==========================================================

// Création du serveur sur le port 80
WebServer server(80);

// ==========================================================
//                  PAGE PRINCIPALE
// ==========================================================

void pageAccueil() {

  // ======================================================
  // AUTHENTIFICATION
  // ======================================================
  //
  // Si login/mot de passe incorrect :
  // le navigateur affiche une popup.
  //
  // ======================================================

  if (!server.authenticate(www_user, www_pass)) {

    return server.requestAuthentication();
  }

  // ======================================================
  // RECUPERATION DES ADRESSES IP
  // ======================================================

  // IPv4 locale
  String ipv4 = WiFi.localIP().toString();

  // IPv6 globale Internet
  String ipv6 = WiFi.globalIPv6().toString();

  // Création URL Internet complète
  String urlInternet = "http://[" + ipv6 + "]";

  // ======================================================
  // CREATION PAGE HTML
  // ======================================================

  String html = "";

  html += "<!DOCTYPE html>";
  html += "<html>";

  // ================= HEAD =================

  html += "<head>";

  html += "<meta charset='UTF-8'>";

  html += "<meta name='viewport' ";
  html += "content='width=device-width,initial-scale=1'>";

  html += "<title>ESP32 IPv6</title>";

  html += "</head>";

  // ================= BODY =================

  html += "<body style='font-family:Arial;";
  html += "background:#0b1f3a;";
  html += "color:white;";
  html += "text-align:center;'>";

  html += "<h1>ESP32 Accessible Internet</h1>";

  // ======================================================
  // IPv4
  // ======================================================

  html += "<p><b>IPv4 locale :</b><br>";
  html += ipv4;
  html += "</p>";

  // ======================================================
  // IPv6
  // ======================================================

  html += "<p><b>IPv6 globale :</b><br>";
  html += ipv6;
  html += "</p>";

  // ======================================================
  // URL INTERNET COMPLETE
  // ======================================================

  html += "<p><b>Adresse Internet :</b><br>";

  html += "<a style='color:lime;' href='";

  html += urlInternet;

  html += "'>";

  html += urlInternet;

  html += "</a>";

  html += "</p>";

  // ======================================================
  // AIDE TELEPHONE
  // ======================================================

  html += "<hr>";

  html += "<p>";
  html += "Ajoute cette page a l'ecran d'accueil";
  html += "<br>";
  html += "pour ne plus taper l'adresse IPv6.";
  html += "</p>";

  html += "</body>";
  html += "</html>";

  // ======================================================
  // ENVOI PAGE WEB
  // ======================================================

  server.send(200, "text/html", html);
}

// ==========================================================
//                        SETUP
// ==========================================================

void setup() {

  // Démarrage port série
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("Demarrage ESP32");

  // ======================================================
  // MODE WIFI STATION
  // ======================================================

  WiFi.mode(WIFI_STA);

  // ======================================================
  // ACTIVATION IPv6
  // ======================================================
  //
  // IMPORTANT
  //
  // Sans cette ligne :
  // pas d'IPv6 globale.
  //
  // ======================================================

  WiFi.enableIPv6();

  // ======================================================
  // CONNEXION WIFI
  // ======================================================

  WiFi.begin(ssid, password);

  Serial.print("Connexion WiFi");

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connecte");

  // ======================================================
  // IPv4
  // ======================================================

  Serial.print("IPv4 locale : ");

  Serial.println(WiFi.localIP());

  // ======================================================
  // ATTENTE IPv6
  // ======================================================
  //
  // On laisse un peu de temps
  // pour recevoir l'IPv6 globale.
  //
  // ======================================================

  delay(1500);

  // ======================================================
  // IPv6
  // ======================================================

  Serial.print("IPv6 globale : ");

  Serial.println(WiFi.globalIPv6());

  // ======================================================
  // URL INTERNET COMPLETE
  // ======================================================

  Serial.print("Adresse Internet : http://[");

  Serial.print(WiFi.globalIPv6());

  Serial.println("]");

  // ======================================================
  // PAGE PRINCIPALE
  // ======================================================

  server.on("/", pageAccueil);

  // ======================================================
  // DEMARRAGE SERVEUR
  // ======================================================

  server.begin();

  Serial.println("Serveur web demarre");
}

// ==========================================================
//                         LOOP
// ==========================================================

void loop() {

  // Gestion serveur web
  server.handleClient();
}