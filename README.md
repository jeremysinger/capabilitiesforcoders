# Capabilities for coders

[![Built with Starlight](https://astro.badg.es/v2/built-with-starlight/tiny.svg)](https://starlight.astro.build)

This is the repository for Capabilities for coders;

This project is build with [starlight](https://starlight.astro.build/), which is an extension for [astro](https://astro.build/).

---

# Building and deploying

### Prerequisites:

To build this site you will need:

1. an installation of nodejs (version 18, 20 or later). I recommend using [NVM](https://github.com/nvm-sh/nvm) to set this up.
2. a JS package manager. This project has been written with [pnpm](https://pnpm.io/installation).

To deploy it, you will need to configure a web server such as apache or nginx to serve the static build files. By default, these will all be generated in `./build/`

### Installing capabilities for coders

Astro is a static site generator; It generates static files which can be served with any regular web server. To build this project:

1. clone the repo
2. install the dependencies
3. build the site

```sh
git clone https://github.com/jeremysinger/capabilitiesforcoders.git
cd capabilities-for-coders-v2
pnpm install
pnpm build
```

---

## Project Structure

Broadly, this site covers CHERI, Morello, and CHERI-BSD. We also have a separate category for common compilers warnings or errors. A folder for each of these can be found in `src/content/docs`; each markdown file will automatically be converted into a corresponding webpage.

In order to get the page to appear on the sidebar for site navigation, you also need to modify the sidebar configuration in `astro.config.mjs`. This should be fairly self explanatory, but check the starlight docs if you need clarification.

Images can be added to `src/assets/` and embedded in Markdown with a relative link.

Static assets, like favicons, can be placed in the `public/` directory.
