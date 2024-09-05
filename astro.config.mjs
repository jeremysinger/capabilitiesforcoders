import { defineConfig } from "astro/config";
import starlight from "@astrojs/starlight";
import starlightLinksValidator from "starlight-links-validator";
import rehypeMermaid from "rehype-mermaid";

// https://astro.build/config
export default defineConfig({
  markdown: {
    rehypePlugins: [[rehypeMermaid, { strategy: "pre-mermaid", dark: true }]],
  },
  integrations: [
    starlight({
      head: [
        {
          tag: "script",
          attrs: { type: "module" },
          content:
            "import mermaid from 'https://cdn.jsdelivr.net/npm/mermaid@11/dist/mermaid.esm.min.mjs'; mermaid.initialize({ startOnLoad: true });",
        },
      ],
      title: "Capabilities for Coders",
      logo: { src: "./src/assets/cherry_bakewell.png" },
      favicon: "/favicon-32x32.png",
      social: {
        github: "https://github.com/JakeTrevor/capabilities-for-coders-v2",
      },
      editLink: {
        baseUrl:
          "https://github.com/JakeTrevor/capabilities-for-coders-v2/edit/main",
      },
      plugins: [starlightLinksValidator()],
      sidebar: [
        {
          label: "Cheri",
          items: [
            "cheri/what-is-cheri",
            "cheri/capabilities",
            {
              collapsed: true,
              label: "Compiling for CHERI",
              items: [
                "cheri/compiling/cross-compiling",
                "cheri/compiling/purecap",
                "cheri/compiling/hybrid",
              ],
            },
            "cheri/running",
            "cheri/header",
            "cheri/purecap-and-hybrid",
            "cheri/printf",
            "cheri/purposes_of_sealed_capabilities",
            "cheri/how_to_seal",
            "cheri/setbounds",
            "cheri/tagcheck",
          ],
        },
        {
          label: "Morello",
          items: [
            "morello/get-a-board",
            "morello/how_to_tell_morello",
            "morello/teardown",
            "morello/python",
            "morello/compartmentalisation",
            "morello/benchmark_abi",
          ],
        },
        {
          label: "CheriBSD",
          items: [
            "cheri-bsd/installing_gdb",
            "cheri-bsd/compile_on_freebsd",
            "cheri-bsd/configure_networking",
            "cheri-bsd/hardware_performance_counters",
            "cheri-bsd/ldpreload",
            "cheri-bsd/shared_library_path",
          ],
        },
        {
          label: "Warnings and Errors",
          items: [
            "warnings-and-errors/capability_misuse",
            "warnings-and-errors/implicit_conversion",
            "warnings-and-errors/unclear_provenance",
          ],
        },
      ],
    }),
  ],
});
