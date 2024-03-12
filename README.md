# webserv

### Description
Serveur HTTP en C++ 98 conforme à la norme.

### Prérequis
- Le serveur ne doit jamais bloquer et doit toujours répondre correctement au client.
- Utilisation exclusive d'un seul poll() (ou équivalent) pour toutes les opérations d'entrée/sortie.
- Le serveur doit être non bloquant, utiliser un mode non bloquant pour les file descriptors.
- Le serveur doit être compatible avec le navigateur web de votre choix.

### Fichier de Configuration
Le fichier de configuration doit permettre de :
- Choisir le port et l'host pour chaque "serveur".
- Configurer les noms de serveur.
- Définir des pages d'erreur par défaut.
- Limiter la taille du corps des requêtes des clients.
- Configurer des routes avec des règles spécifiques : méthodes HTTP acceptées, redirections, répertoires de recherche, etc.

### Stress Test
Le serveur doit rester disponible et ne jamais cesser de fonctionner, indépendamment de la charge.

## Installation et Exécution

### Compilation
Utilisez le `Makefile` fourni pour compiler le projet :
```bash
make
./webserv
```

## Fichier de Configuration

Le fichier de configuration de `webserv` permet de définir le comportement du serveur HTTP selon différentes directives.
Il doit se trouver dans le dossier asset/ et doit toujours être nommé webserv.conf.
Voici un aperçu des possibilités offertes par le fichier de configuration :

```yaml
server {
    listen 127.0.0.1:3003;             # Adresse et port sur lesquels le serveur écoute.
    server_name localhost;             # Nom du serveur.

    limit_except GET;                  # Méthodes HTTP autorisées.
    root dist/www;                     # Dossier racine pour la recherche des fichiers.
    autoindex off;                     # Activation ou désactivation de l'indexation automatique.

    # Directive de taille maximale du corps de requête client.
    # client_max_body_size 1;

    location /upload {
        limit_except POST;             # Méthodes HTTP autorisées pour cette route.
        upload_store dist/uploads;     # Emplacement de stockage pour les fichiers téléversés.
        client_max_body_size 10G;      # Taille maximale du corps de requête client pour cette route.
    }

    location /uploadlimit {
        limit_except POST;
        upload_store dist/uploads;
        client_max_body_size 5;        # Taille maximale réduite pour tester les limites.
    }

    # Redirections avec différents codes de statut HTTP.
    location /redirmultiple {
        return 300 Http://www.42.fr https://www.42mulhouse.fr/;
    }
    location /redirpermanente {
        return 301 https://www.42mulhouse.fr/;
    }
    location /redirtemp {
        return 307 https://www.42mulhouse.fr/;
    }

    # Configuration pour le téléchargement de fichiers.
    location /download {
        root dist/download;
        index fr.subject.pdf;          # Fichier par défaut à servir.
    }

    # Configuration pour exécuter des scripts CGI Python.
    location /python-cgi {
        limit_except GET POST;
        root dist/cgi/python;
        cgi_py /usr/bin/python3;        # Chemin vers l'interpréteur Python.
        index test.py;                  # Script par défaut à exécuter.
    }

    # Gestion personnalisée des erreurs HTTP.
    location /testcustom404 {
        root dist/www/testcustom404;
        error_page 404 /404.html;
        autoindex off;
    }

    # Activation de l'indexation automatique pour une route spécifique.
    location /autoindex {
        root dist/www/autoindex;
        autoindex on;
    }

    location /error {
        limit_except POST;
    }

    # Configuration pour accepter les requêtes DELETE.
    location /delete {
        root dist/www/delete;
        limit_except DELETE;
    }
}

# Exemple de configuration pour un second serveur (commenté).
# server {
#     listen 172.24.230.105:3004;
#     server_name localhost;

#     root dist;
# }
````

