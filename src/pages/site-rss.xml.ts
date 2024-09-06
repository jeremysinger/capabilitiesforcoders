import type { APIContext } from "astro";
import rss from "@astrojs/rss";
import { getCollection } from "astro:content";

export async function GET(context: APIContext) {
  const pages = await getCollection("docs");

  return rss({
    title: "Capabilities for Coders",
    description:
      "All you wanted to know about capabilities but were afraid to ask",
    site: context.site!,
    items: pages.map((e) => ({
      title: e.data.title,
      description: e.data.description,
      link: e.slug,
      pubDate: new Date(Date.now()),
      // Other fields here could be a good idea.
    })),
  });
}
