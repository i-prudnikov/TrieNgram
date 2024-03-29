## This project shows implementation of Markov chain, for generating text based on n-gramms.
The overall logic is following:
1. Text corpus is loaded into memory and trie structure is created, with the specified
Markov chain rank (n)
2. Sample text is provided, with the parameters, that used to generating text, like:
 - count of words to be added
 - method of choosing words in n-grams: most frequent or probability distribution.

##

#### The tool is checked against Lev Tolstoy's `Война и миръ`. Here the funny outcome from start 3-gram `анна павловна соображая` with `chain rank = 3` & 2 different distribution types. The total length of generated text set to 100 words (punctuation removed and all words are lowercased).


#### Most frequent:
- анна павловна соображая я нынче же поговорю lise la femme du jeune болконский с лизой женой молодого болконского и может быть я и отказала может быть с болконским всё кончено почему ты думаешь про меня так дурно я ничего не знаю и не буду ежели бы бонапарте стоял тут у смоленска угрожая лысым горам и тогда бы я не знал что он жив опять с одной стороны стола сидели анатоль долохов несвицкий денисов и другие такие же категория этих людей так же ясно была во сне что благодетель говорил о добре о возможности быть тем чем были они и они со всех сторон
---
 
#### Prob distrib (3 different variants):
- анна павловна соображая я нынче же поговорю lise la femme du jeune болконский с лизой женой молодого болконского и может быть это уладится ce sera dans votre famille que je ferai mon apprentissage de vieille fille я в вашем семействе начну обучаться ремеслу старой девки ii гостиная анны павловны начала понемногу наполняться приехала высшая знать петербурга люди самые разнородные по возрастам и характерам но одинаковые по обществу в каком все жили приехала дочь князя василия красавица элен заехавшая за отцом чтобы с ним переговорить действовать и помогать войскам истреблять злодеев станем и мы из них дух продолжал чтец и остановился видал победоносно прокричал
---
- анна павловна соображая я нынче же поговорю lise la femme du jeune болконский с лизой женой молодого болконского и может быть я часто думаю она была слишком молода а он был почти постоянно физически действовало на пьера оно связывало все его члены уничтожало бессознательность и свободу его движений такая странная антипатия думал пьер а этот человек знает истину и ежели бы ему позволил седок что это как баг'аны точь в точь так же вздрагивал и нагибался вперед гусарский офицер несвицкий смеялся и толкал других чтобы они смотрели на графа и обмануть его южная весна покойное быстрое путешествие в венской коляске и уединение дороги
---
- анна павловна соображая я нынче же поговорю lise la femme du jeune болконский с лизой женой молодого болконского и может быть я бы то же если бы нам только приходилось воевать с мужиками мы бы не ходили драться в австрию и в пруссию сами не зная что ему теперь с ним делать не беспокойтесь сказал пьер я не понимаю продолжал старик кто будет землю пахать коли им волю дать легко законы писать а управлять трудно всё равно как теперь я вас спрашиваю сказала она строго я что я сказал пьер чувствуя необходимость умалить как возможно свое общественное положение чтобы быть ближе и понятнее
---


#### Build
If you use code::blocks, just open the project in the IDE and build it.
Otherwise use compilation flags: `-std=c++14 -licuuc -licuio`.
The latter is for UTF-8 text support https://unicode-org.github.io/icu-docs/apidoc/released/icu4c/
