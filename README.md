# About the Project
This is a completely "clean room" untainted reverse engineered "SDK" for the VST 2.x interface. It was reverse engineered from binaries where no license restricting the reverse engineering was attached, or where the legal system explicitly allowed reverse engineering for the purpose of interoperability.

## At a glance
- Full reimplementation of the VST 2.x SDK.
- Header-only, so no code/object dependencies!
- Written for C99 / C++20 with (optional) backwards support down to C89 / C++98.
- Documentation available [online](https://xaymar.github.io/vst2sdk) _and_ offline with clean JavaDoc comments.
- Liberally licensed so it is usable in commercial products.
  - Technically as clean room as it can legally get, but ask a lawyer first anyway.
  - I don't provide licenses to Steinberg VsT, their logos, or anything.

## As seen in
[DISTRHO Plugin Framework](https://github.com/DISTRHO/DPF), [StompTuner](https://github.com/brummer10/StompTuner), [DF Zita Rev1](https://github.com/SpotlightKid/dfzitarev1), [TonPlugIns](https://github.com/Vhonowslend/TonPlugIns), [ToneTwistPlugs](https://github.com/brummer10/ToneTwistPlugs), [DF JPverb](https://github.com/SpotlightKid/dfjpverb), [CChorus](https://github.com/SpotlightKid/cchorus), [Dynation](https://github.com/vertver/Dynation) and likely more.

*(Want to be listed here? Open an Issue or Pull Request!)*

# Frequently Asked Questions
## Is this legal? Can I use this in my own product?
**Disclaimer:** I am not a lawyer. Consult a real lawyer before including this in a paid product. The information presented below is purely from available copyright laws that I could find about this topic. You should always consult with a lawyer first before including this in your product.

As this only enables interoperability with existing VST 2.x programs and addons, it is considered to be reverse engineering in the name of interoperability. In most of the developed world, this is considered completely legal and is fine to be used by anyone, as long as it is not the only function of the product.

Note that this does not grant any patent licenses, nor does it grant you any right to use trademarks in the names. That could mean that you can't advertise your product as having support for VST, and can't use VST in the name or presentation of the product at all unless you have permission to do so.

## Why recreate an SDK for something officially abandoned by the creators?
There is a ton of software that is only capable of loading VST2.x audio effects, and Steinberg has made no effort to create a VST3-to-VST2-adapter for that software. Notable software includes Audacity and OBS Studio, which both likely felt restricted by the license additions Steinberg added to the GPL license.

## What was used to create this clean room project?
**Disclaimer:** I am not a lawyer. Consult a real lawyer before including this in a paid product.

The initial version was created through the best available legal clean room reverse engineering method available. The whole process went through several rounds of the "telephone" game involving external developers and lawyers before the final document arrived on my desk. This initial version was barely functional as a lot of context ended up missing resulting in quite basic functionality.

This version went through several development cycles before settling on what I've now labelled as `v0.1.1`. It was stable and worked for the most part but lacked a lot of features. I've used it as a base for my own plug-ins and had quite a bit of success with it but the incompleteness of the recreated SDK was noticable. Eventually I recreated VST 3.x SDK compatible headers using the VST 3.x SDK which caused the license to upgrade from BSD 2-clause to BSD 3-clause - now known as `v0.2.0`.

As of late July/early August 2025, I had a found a bit of interest in this SDK again and didn't want to leave it in the incomplete state it was. I've recreated the fake plug-in and host that I used before and decided to automate behavior scanning: What do hosts or plug-ins expect us to do? What do they call? Etc. This was then let loose on several thousand available plug-ins and has resulted in the `v0.3.0` update to the SDK. As far as I can tell the SDK now has almost all functionality that VST 2.x plug-ins and hosts could want.

There's still a bit of work to do but it won't be easy as almost all of it involves structured data for which I can only guess. Progress is being made, but it's so much slower compared to when I had pages of telephone game documentation to work off of. Still better than nothing!
