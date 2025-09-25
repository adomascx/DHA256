# VU Blockchain 1 Laboratorinis Darbas

## Komandos Nariai

**Mūsų Komanda:**

- Adomas Lukoševičius (PHA256 - Rankinis realizavimas + AI pagerintas variantas)
- Nojus Petrušis (Nojaus Hash - Rankinis realizavimas)

**Palyginimo Komanda:**

- Valentinas Šamatovičius
- Andrius Rakauskas

## Projekto Apžvalga

Šis projektas realizuoja originalias hash funkcijas VU Blockchain kurso 1 laboratoriniui darbui.
Tikslas - sukurti originalų hash generatorių, kuris atitiktų kriptografinių hash funkcijų standartus ir nekopijuotų esamų algoritmų kaip SHA-256 ar MD5.

## Algoritmo pseudokodai

### PHA256 (Adomo algoritmas)

```pseudocode
FUNCTION PHA256(input):
    x = 0, y = 0
    
    FOR each char IN input:
        x += cos(char)
        y += sin(char)
    
    RETURN float_to_hex32(x) + float_to_hex32(y)

FUNCTION float_to_hex32(value):
    value = abs(value)
    
    // Normalizuoti į (1,10) intervalą
    normalized = value / 10^floor(log10(value))
    
    // Ištraukti 40 skaitmenų į du 64-bit sveikuosius skaičius
    acc1 = extract_first_20_digits(normalized)
    acc2 = extract_next_20_digits(normalized)
    
    // Konvertuoti į 32 simbolių hex string
    RETURN format_hex(acc1, 16) + format_hex(acc2, 16)
```

### Nojaus Hash

```pseudocode
FUNCTION NojausHash(input):
    // TODO: Pridėti Nojaus hash algoritmo pseudo-kodą
    
    // Grąžina 64 simbolių hex eilutę
    RETURN hash_output
```

## Realizavimo Detalės

### Kompiliavimo Instrukcijos

#### PHA256 ir PHA256_AI (Adomas)

```bash
cd Adomas/
make test
```

#### Nojaus Hash (Nojus)

```bash
cd Nojus/
make
```

### Naudojimas **[TBD]**

#### PHA256 ir PHA256_AI (Adomas)

```bash
# PHA256 (rankinis) - failo įvedimas
./pha256 input_file.txt

# PHA256 (rankinis) - interaktyvus įvedimas
./pha256
# Įveskite tekstą rankiniu būdu, kai bus paprašyta

# PHA256_AI (pagerintas) - failo įvedimas
./pha256_ai input_file.txt

# PHA256_AI (pagerintas) - interaktyvus įvedimas
./pha256_ai
# Įveskite tekstą rankiniu būdu, kai bus paprašyta
```

#### Nojaus Hash (Nojus)

```bash
# Failo įvedimas
./nojaus_hash input_file.txt

# Interaktyvus įvedimas
./nojaus_hash
# Įveskite tekstą rankiniu būdu, kai bus paprašyta
```

## Eksperimentinis Tyrimas

### 1. Teisingo Išvedimo Testavimas

**Testiniai Failai:**

- Vieno simbolio failai (`a`, `b`)
- Dideli atsitiktiniai failai (>1000 simbolių)
- Failai, skirtingai vienu simboliu
- Tuščias failas

**Rezultatai:**

| Įvedimo Tipas   | PHA256 Išvedimo Ilgis | PHA256_AI Išvedimo Ilgis | Nojaus Hash Išvedimo Ilgis |
| --------------- | --------------------- | ------------------------ | -------------------------- |
| Vienas simbolis | 64 hex simboliai      | 64 hex simboliai         | 64 hex simboliai           |
| Didelis failas  | 64 hex simboliai      | 64 hex simboliai         | 64 hex simboliai           |
| Tuščias failas  | 64 hex simboliai      | 64 hex simboliai         | 64 hex simboliai           |

### 2. Deterministiškumo Testavimas

**Metodas:** Tas pats input hash'inamas kelis kartus

**Rezultatai:**

| Algoritmas  | Deterministinis? |
| ----------- | ---------------- |
| PHA256      | Taip             |
| PHA256_AI   | Taip             |
| Nojaus Hash | *Taip/Ne*        |

### 3. Veikimo Efektyvumo Analizė

**Testinis Failas:** `konstitucija.txt`

#### PHA256 Greitis (rankinis)

| Apdorotų Eilučių | Laikas (ms) | Hash Greitis (eilutės/sek) |
| ---------------- | ----------- | -------------------------- |
| 1                |             |                            |
| 2                |             |                            |
| 4                |             |                            |
| 8                |             |                            |
| 16               |             |                            |

#### PHA256_AI Greitis (pagerintas)

| Apdorotų Eilučių | Laikas (ms) | Hash Greitis (eilutės/sek) |
| ---------------- | ----------- | -------------------------- |
| 1                |             |                            |
| 2                |             |                            |
| 4                |             |                            |
| 8                |             |                            |
| 16               |             |                            |

#### *Nojaus Hash Greitis*

| Apdorotų Eilučių | Laikas (ms) | Hash Greitis (eilutės/sek) |
| ---------------- | ----------- | -------------------------- |
| 1                |             |                            |
| 2                |             |                            |
| 4                |             |                            |
| 8                |             |                            |
| 16               |             |                            |

**Veikimo Grafikas:**

[TODO: Pridėti grafikus visiems trims algoritmams]

### 4. Atsparumo Kolizijoms Testavimas

**Metodas:** Generuoti 100,000 atsitiktinių eilučių porų kiekvienai ilgio kategorijai

#### PHA256 Kolizijos (rankinis)

| Eilutės Ilgis | Išbandytų Porų | Rastų Kolizijų | Kolizijų Dažnis |
| ------------- | -------------- | -------------- | --------------- |
| 10 simbolių   | 100,000        |                |                 |
| 100 simbolių  | 100,000        |                |                 |
| 500 simbolių  | 100,000        |                |                 |
| 1000 simbolių | 100,000        |                |                 |

#### PHA256_AI Kolizijos (pagerintas)

| Eilutės Ilgis | Išbandytų Porų | Rastų Kolizijų | Kolizijų Dažnis |
| ------------- | -------------- | -------------- | --------------- |
| 10 simbolių   | 100,000        |                |                 |
| 100 simbolių  | 100,000        |                |                 |
| 500 simbolių  | 100,000        |                |                 |
| 1000 simbolių | 100,000        |                |                 |

#### Nojaus Hash Kolizijos

| Eilutės Ilgis | Išbandytų Porų | Rastų Kolizijų | Kolizijų Dažnis |
| ------------- | -------------- | -------------- | --------------- |
| 10 simbolių   | 100,000        |                |                 |
| 100 simbolių  | 100,000        |                |                 |
| 500 simbolių  | 100,000        |                |                 |
| 1000 simbolių | 100,000        |                |                 |

### 5. Lavinos Efekto Analizė

**Metodas:** 100,000 porų, skirtingų vienu simboliu

#### PHA256 Lavinos Efektas (rankinis)

- **Minimalus skirtumas:** % bit'ų pakeista
- **Maksimalus skirtumas:** % bit'ų pakeista
- **Vidutinis skirtumas:** % bit'ų pakeista
- **Tikėtinas (idealus):** ~50% bit'ų pakeista

#### PHA256_AI Lavinos Efektas (pagerintas)

- **Minimalus skirtumas:** % bit'ų pakeista
- **Maksimalus skirtumas:** % bit'ų pakeista
- **Vidutinis skirtumas:** % bit'ų pakeista
- **Tikėtinas (idealus):** ~50% bit'ų pakeista

#### Nojaus Hash Lavinos Efektas

- **Minimalus skirtumas:** % bit'ų pakeista
- **Maksimalus skirtumas:** % bit'ų pakeista
- **Vidutinis skirtumas:** % bit'ų pakeista
- **Tikėtinas (idealus):** ~50% bit'ų pakeista

**Lavinos Efekto Grafikai:**
[TODO: Pridėti bit'ų skirtumų histogramas visiems trims algoritmams]

### 6. Negrįžtamumo Savybės Demonstracija

**Metodas:** Hash su druska `HASH(input + salt)`

**PHA256 Analizė (rankinis):** [TODO: Aprašyti reverse engineering sudėtingumą]

**PHA256_AI Analizė (pagerintas):** [TODO: Aprašyti reverse engineering sudėtingumą]

**Nojaus Hash Analizė:** [TODO: Aprašyti reverse engineering sudėtingumą]

## Palyginimo Rezultatai

### PHA256 Vidinės Komandos Palyginimas (Rankinis vs AI-Pagerintas)

| Metrika               | Rankinis (v0.1) | AI-Pagerintas (v0.2) | Pagerinimas |
| --------------------- | --------------- | -------------------- | ----------- |
| Greitis               |                 |                      |             |
| Atsparumas Kolizijoms |                 |                      |             |
| Lavinos Efektas       |                 |                      |             |

### Visų Trijų Algoritmų Palyginimas

| Algoritmas        | Greitis | Kolizijų Dažnis | Lavinos % |
| ----------------- | ------- | --------------- | --------- |
| PHA256 (Rankinis) |         |                 |           |
| PHA256_AI         |         |                 |           |
| Nojaus Hash       |         |                 |           |

### Palyginimas su Kitos Komandos Algoritmu

| Algoritmas              | Greitis | Kolizijų Dažnis | Lavinos % |
| ----------------------- | ------- | --------------- | --------- |
| PHA256 (Rankinis)       |         |                 |           |
| PHA256_AI               |         |                 |           |
| Nojaus Hash             |         |                 |           |
| Valentinas/Andrius Algo |         |                 |           |

### Palyginimas su Standartiniais Algoritmais

| Algoritmas  | Greitis (eilutės/sek) | Atsparumas Kolizijoms | Lavinos Efektas |
| ----------- | --------------------- | --------------------- | --------------- |
| PHA256      |                       |                       |                 |
| PHA256_AI   |                       |                       |                 |
| Nojaus Hash |                       |                       |                 |
| SHA-256     |                       |                       |                 |
