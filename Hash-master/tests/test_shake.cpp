/*
 *  Chocobo1/Hash
 *
 *   Copyright 2017-2018 by Mike Tzou (Chocobo1)
 *     https://github.com/Chocobo1/Hash
 *
 *   Licensed under GNU General Public License 3 or later.
 *
 *  @license GPL3 <https://www.gnu.org/licenses/gpl-3.0-standalone.html>
 */

#include "../src/sha3.h"

#include "catch2/single_include/catch2/catch.hpp"

#include <cstring>


TEST_CASE("shake-256")  // NOLINT
{
	using Hash = Chocobo1::SHAKE_256;

	// official test suite from NIST
	const char s1[] = "";
	REQUIRE("46b9dd2b0ba88d13233b3feb743eeb243fcd52ea62b81b82b50c27646ed5762fd75dc4ddd8c0f200cb05019d67b592f6fc821c49479ab48640292eacb3b7c4be141e96616fb13957692cc7edd0b45ae3dc07223c8e92937bef84bc0eab862853349ec75546f58fb7c2775c38462c5010d846c185c15111e595522a6bcd16cf86f3d122109e3b1fdd943b6aec468a2d621a7c06c6a957c62b54dafc3be87567d677231395f6147293b68ceab7a9e0c58d864e8efde4e1b9a46cbe854713672f5caaae314ed9083dab4b099f8e300f01b8650f1f4b1d8fcf3f3cb53fb8e9eb2ea203bdc970f50ae55428a91f7f53ac266b28419c3778a15fd248d339ede785fb7f5a1aaa96d313eacc890936c173cdcd0fab882c45755feb3aed96d477ff96390bf9a66d1368b208e21f7c10d04a3dbd4e360633e5db4b602601c14cea737db3dcf722632cc77851cbdde2aaf0a33a07b373445df490cc8fc1e4160ff118378f11f0477de055a81a9eda57a4a2cfb0c83929d310912f729ec6cfa36c6ac6a75837143045d791cc85eff5b21932f23861bcf23a52b5da67eaf7baae0f5fb1369db78f3ac45f8c4ac5671d85735cdddb09d2b1e34a1fc066ff4a162cb263d6541274ae2fcc865f618abe27c124cd8b074ccd516301b91875824d09958f341ef274bdab0bae316339894304e35877b0c28a9b1fd166c796b9cc258a064a8f57e27f2a"
			== Hash(512).addData(s1, strlen(s1)).finalize().toString());

	const unsigned char s2 = 0xa3;
	Hash test2(512);
	for (long int i = 0 ; i < 200; ++i)
		test2.addData(&s2, 1);
	REQUIRE("cd8a920ed141aa0407a22d59288652e9d9f1a7ee0c1e7c1ca699424da84a904d2d700caae7396ece96604440577da4f3aa22aeb8857f961c4cd8e06f0ae6610b1048a7f64e1074cd629e85ad7566048efc4fb500b486a3309a8f26724c0ed628001a1099422468de726f1061d99eb9e93604d5aa7467d4b1bd6484582a384317d7f47d750b8f5499512bb85a226c4243556e696f6bd072c5aa2d9b69730244b56853d16970ad817e213e470618178001c9fb56c54fefa5fee67d2da524bb3b0b61ef0e9114a92cdbb6cccb98615cfe76e3510dd88d1cc28ff99287512f24bfafa1a76877b6f37198e3a641c68a7c42d45fa7acc10dae5f3cefb7b735f12d4e589f7a456e78c0f5e4c4471fffa5e4fa0514ae974d8c2648513b5db494cea847156d277ad0e141c24c7839064cd08851bc2e7ca109fd4e251c35bb0a04fb05b364ff8c4d8b59bc303e25328c09a882e952518e1a8ae0ff265d61c465896973d7490499dc639fb8502b39456791b1b6ec5bcc5d9ac36a6df622a070d43fed781f5f149f7b62675e7d1a4d6dec48c1c7164586eae06a51208c0b791244d307726505c3ad4b26b6822377257aa152037560a739714a3ca79bd605547c9b78dd1f596f2d4f1791bc689a0e9b799a37339c04275733740143ef5d2b58b96a363d4e08076a1a9d7846436e4dca5728b6f760eef0ca92bf0be5615e96959d767197a0beeb"
			== test2.finalize().toString());


	// my own tests
	REQUIRE(Hash(512) == Hash(512));
	REQUIRE(Hash(512).addData("123").finalize() != Hash(512).finalize());

	REQUIRE("46b9dd2b0ba88d13233b3feb743eeb243fcd52ea62b81b82b50c27646ed5762fd75dc4ddd8c0f200cb05019d67b592f6fc821c49479ab48640292eacb3b7c4be141e96616fb13957692cc7edd0b45ae3dc07223c8e92937bef84bc0eab862853349ec75546f58fb7c2775c38462c5010d846c185c15111e595522a6bcd16cf86f3d122109e3b1fdd943b6aec468a2d621a7c06c6a957c62b54dafc3be87567d677231395f6147293b68ceab7a9e0c58d864e8efde4e1b9a46cbe854713672f5caaae314ed9083dab4b099f8e300f01b8650f1f4b1d8fcf3f3cb53fb8e9eb2ea203bdc970f50ae55428a91f7f53ac266b28419c3778a15fd248d339ede785fb7f5a1aaa96d313eacc890936c173cdcd0fab882c45755feb3aed96d477ff96390bf9a66d1368b208e21f7c10d04a3dbd4e360633e5db4b602601c14cea737db3dcf722632cc77851cbdde2aaf0a33a07b373445df490cc8fc1e4160ff118378f11f0477de055a81a9eda57a4a2cfb0c83929d310912f729ec6cfa36c6ac6a75837143045d791cc85eff5b21932f23861bcf23a52b5da67eaf7baae0f5fb1369db78f3ac45f8c4ac5671d85735cdddb09d2b1e34a1fc066ff4a162cb263d6541274ae2fcc865f618abe27c124cd8b074ccd516301b91875824d09958f341ef274bdab0bae316339894304e35877b0c28a9b1fd166c796b9cc258a064a8f57e27f2a"
			== Hash(512).finalize().toString());

	const char s3[] = "a";
	Hash test3(512);
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("59b187c01a1704d2942526f0ab59866dbf0d81736d37960732c911c9e99cf001315b076e491ad6da9ded9e7388b9675c4dc012a86855622894950849e1698aa789cbc3eb5cfd139bc1bec25271d23cbc770e7395ffd19c06bab2d11ff7b87db0e7feed588af8f4ee59505c749ab4408fcd5568794ad8fc916a3b46ef0ad8b8a6ade24f82a4555abeedeed33862a024efd6605337a40f27a43b5283ef1221c5b77989cc4fc8834657483dd2d89449cf4da6d05b3b48b16c3d6741494443a0fc8a29e6aa59d5fae8c443f9865fe5a0c7b522d891b907af1f53125b817adf40dc10c3062e70692ddd5648828e8b0cb587541cd82a705240af111caca2e0786269e6dd65341653fd3c229804c5ee26af7b9622101b5e7a175999599850b7cba15981466d86c081f8f777e98e4c14d6f38868d0c5d1ba307ff2fe8d56e6fedcc93eae43157146388769a53595e026326bb30d55aa8919e80621577df219f7b93fb3cdb30785f77cf3582ea91bfdd395c8e1c4de25106613a4f7c195ee351fc300eb46e525d9735347fe82fbf98e7c6f77dde6bfd891ca6eabb17dee495aa4e86544352768041e1e4de05658ceda39b0c0491f8bb03f7f14e344618002b9fe9e564eb7bf54cb0b99ed40a8c856d82f9b934f6c7b771364ad132f6152898396944b189c37a4b211058b5b25a05c2623b399d5048541a477f3a08764b1cc46dfdecbf0d7"
			== test3.finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("2f671343d9b2e1604dc9dcf0753e5fe15c7c64a0d283cbbf722d411a0e36f6ca1d01d1369a23539cd80f7c054b6e5daf9c962cad5b8ed5bd11998b40d5734442bed798f6e5c915bd8bb07e0188d0a55c1290074f1c287af06352299184492cbdec9acba737ee292e5adaa445547355e72a03a3bac3aac770fe5d6b66600ff15d37d5b4789994ea2aeb097f550aa5e88e4d8ff0ba07b88c1c88573063f5d96df820abc2abd177ab037f351c375e553af917132cf2f563c79a619e1bb76e8e2266b0c5617d695f2c496a25f4073b6840c1833757ebb386f16757a8e16a21e9355e9b248f3b33be672da700266be99b8f8725e8ab06075f0219e655ebc188976364b7db139390d34a6ea67b4b223229183a94cf455ece91fdaf5b9c707fa4b40ec39816c1120c7aaaf47920977be900e6b9ca4b8940e192b927c475bd58e836f512ae3e52924e36ff8e9b1d0251047770a5e465905622b1f159be121ab93819c5e5c6dae299ac73bf1c4ed4a1e2c7fa3caa1039b05e94c9f993d04feb272b6e00bb0276939cf746c42936831fc8f2b4cb0cf94808ae0af405ce4bc67d1e7acfc6fd6590d3de91f795df5aaf57e2cee1845a303d0ea564be3f1299acdce67efe0d62cfc6d6829ff4ecc0a05153c24696c4d34c076453827e796f3062f94f62f4528b7cfc870f0dcd615b7c97b95da4b9be5830e8b3f66cce71e0f622c771994443e2"
			== Hash(512).addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("bd225bfc8b255f3036f0c8866010ed0053b5163a3cae111e723c0c8e704eca4e5d0f1e2a2fa18c8a219de6b88d5917ff5dd75b5fb345e7409a3b333b508a65fb1fd2849dc4f0a7195abce099dc6ac265012f542d628417605452f97a67eb737c91b75021a8823a8d04a2a0f26530fe4d42b41be8830cabf5929b43bb4cafc6350b4105f852f1ef2b536ea572a1eae0bd2c77a005b3499806a03b986c3b53f61af81e5f9cf5c010d10eea427793ba0100ee1a89864708b979ca55381c2bfb8d105b6c0d4a253a13bf4b42c3218ee1a9da4418403e81ef947e99e6ff2b6867d08de4a59e4f504c9768ad26034561b694340d63ca539e5a95b5ef1c4f79e8166bbd6099d236fec1a97bf1f5f98c3992705350bc3caff31b3e2bfecea243d07f4fba52e9a288437b8ae7e72d5c6a891fb0a3fc8e80e575aca43dfdcdc80e178825c039507e21a9174075db62e1aa8bac8802191c8a01a2547c6128682afc3efad23af537bad3762ae3fa96d5b7200e83b0865822007ccb26981984974ecfefc69aa33812f446a2d162dc33dd2c00e1967081a1adfac626f25b339afc38865ffa5c52179594a0eabf946a3e8422e8fb09177961e2000c98d098e570e7f0c9a4e972268f1bf20d19978525912dbb9a6349da531f77d6c90f159ca5794b6f53f5cee73345d0f59a527cbc4282b64f178e016f08983370e4d998317113dc02c6d6f31a34"
			== Hash(512).addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("29378402834e397cfb5fa971b7721d816db11f27467394d37369fb7cef7831a7abf2355e9079d1f4ee291096d1d37e7859458fb19869eb5cd6e2bba1fc292677201b1b75575954551c8cf4d962b50169b6298e73b4ffff6e82162e180e5788a09ba318c094379c1d193d76017637a2b8b8d7912ef1c1f59724610cc3aa0fda55ed86ac4a4871a140d92f082f371026606dbd294594267a23962fc4fb3e3a8d11c2243e9c73774f89c4b2d7a3d216ac27cfcdda9b56c5edb9fca0042d820a69fc39bc28e5ba7173f9ef08f48c9d6de4a48fe7e7633d6384a548c1e28a66e1b3146ec103c8d41beb3788d41f7b47e944046c0a42abe1130abaa46e878f5580eeacf012d6133a8001b61c7669c5363f1a2a1e8fdac2ebbd1b0b86222bcda1f4351dd3e8a638c09e1ffad8f194cb7566e787781fcf2c48d32368f53f6205d40b86be6b355685abd6c212a849c7e670552e64ffd049a5a4104e7936a7e0962c19a51280e03badbb4bdc1edfada88dc1563d83c1bf4c6ae4f3f0af3ff41500dcd8e7ec73e4058bededd31a3f693c159831a68425e05804af237cd16b2efd147a021db261b19a1263d032c2ac47cbaf844f2fabd2c63164a917c1643f6a4251db98147e4fbb7269fd0442d6bdf0b04068508a00e67720f598b79ce2082f38c0e2d5f4e28178714ee3b4519ab2b53ce9a9f39b5bd55427bf69848219543d68747bb047ab"
			== Hash(512).addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(73, 'a');
	REQUIRE("59b187c01a1704d2942526f0ab59866dbf0d81736d37960732c911c9e99cf001315b076e491ad6da9ded9e7388b9675c4dc012a86855622894950849e1698aa789cbc3eb5cfd139bc1bec25271d23cbc770e7395ffd19c06bab2d11ff7b87db0e7feed588af8f4ee59505c749ab4408fcd5568794ad8fc916a3b46ef0ad8b8a6ade24f82a4555abeedeed33862a024efd6605337a40f27a43b5283ef1221c5b77989cc4fc8834657483dd2d89449cf4da6d05b3b48b16c3d6741494443a0fc8a29e6aa59d5fae8c443f9865fe5a0c7b522d891b907af1f53125b817adf40dc10c3062e70692ddd5648828e8b0cb587541cd82a705240af111caca2e0786269e6dd65341653fd3c229804c5ee26af7b9622101b5e7a175999599850b7cba15981466d86c081f8f777e98e4c14d6f38868d0c5d1ba307ff2fe8d56e6fedcc93eae43157146388769a53595e026326bb30d55aa8919e80621577df219f7b93fb3cdb30785f77cf3582ea91bfdd395c8e1c4de25106613a4f7c195ee351fc300eb46e525d9735347fe82fbf98e7c6f77dde6bfd891ca6eabb17dee495aa4e86544352768041e1e4de05658ceda39b0c0491f8bb03f7f14e344618002b9fe9e564eb7bf54cb0b99ed40a8c856d82f9b934f6c7b771364ad132f6152898396944b189c37a4b211058b5b25a05c2623b399d5048541a477f3a08764b1cc46dfdecbf0d7"
			== Hash(512).addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash(512).addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash(512).addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash(512).addData(s16, 2).finalize().toVector();
	const auto s16_2 = Hash(512).addData(s16).finalize().toVector();
	REQUIRE(s16_1 == s16_2);

	REQUIRE(0x46b9dd2b0ba88d13 == std::hash<Hash> {}(Hash(512).finalize()));

	Hash h17(512);
	Hash h17_2(512);
	h17 = h17_2;
	REQUIRE(h17.finalize().toVector() == h17_2.finalize().toVector());
	h17 = std::move(h17_2);
	REQUIRE(h17.finalize().toVector() == Hash(512).finalize().toVector());

	Hash h18(512);
	Hash::BaseType h18_2(512);
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash::BaseType(512).finalize().toVector());
}


TEST_CASE("shake-128")  // NOLINT
{
	using Hash = Chocobo1::SHAKE_128;

	// official test suite from NIST
	const char s1[] = "";
	REQUIRE("7f9c2ba4e88f827d616045507605853ed73b8093f6efbc88eb1a6eacfa66ef263cb1eea988004b93103cfb0aeefd2a686e01fa4a58e8a3639ca8a1e3f9ae57e235b8cc873c23dc62b8d260169afa2f75ab916a58d974918835d25e6a435085b2badfd6dfaac359a5efbb7bcc4b59d538df9a04302e10c8bc1cbf1a0b3a5120ea17cda7cfad765f5623474d368ccca8af0007cd9f5e4c849f167a580b14aabdefaee7eef47cb0fca9767be1fda69419dfb927e9df07348b196691abaeb580b32def58538b8d23f87732ea63b02b4fa0f4873360e2841928cd60dd4cee8cc0d4c922a96188d032675c8ac850933c7aff1533b94c834adbb69c6115bad4692d8619f90b0cdf8a7b9c264029ac185b70b83f2801f2f4b3f70c593ea3aeeb613a7f1b1de33fd75081f592305f2e4526edc09631b10958f464d889f31ba010250fda7f1368ec2967fc84ef2ae9aff268e0b1700affc6820b523a3d917135f2dff2ee06bfe72b3124721d4a26c04e53a75e30e73a7a9c4a95d91c55d495e9f51dd0b5e9d83c6d5e8ce803aa62b8d654db53d09b8dcff273cdfeb573fad8bcd45578bec2e770d01efde86e721a3f7c6cce275dabe6e2143f1af18da7efddc4c7b70b5e345db93cc936bea323491ccb38a388f546a9ff00dd4e1300b9b2153d2041d205b443e41b45a653f2a5c4492c1add544512dda2529833462b71a41a45be97290b6f"
			== Hash(512).addData(s1, strlen(s1)).finalize().toString());

	const unsigned char s2 = 0xa3;
	Hash test2(512);
	for (long int i = 0 ; i < 200; ++i)
		test2.addData(&s2, 1);
	REQUIRE("131ab8d2b594946b9c81333f9bb6e0ce75c3b93104fa3469d3917457385da037cf232ef7164a6d1eb448c8908186ad852d3f85a5cf28da1ab6fe3438171978467f1c05d58c7ef38c284c41f6c2221a76f12ab1c04082660250802294fb87180213fdef5b0ecb7df50ca1f8555be14d32e10f6edcde892c09424b29f597afc270c904556bfcb47a7d40778d390923642b3cbd0579e60908d5a000c1d08b98ef933f806445bf87f8b009ba9e94f7266122ed7ac24e5e266c42a82fa1bbefb7b8db0066e16a85e0493f07df4809aec084a593748ac3dde5a6d7aae1e8b6e5352b2d71efbb47d4caeed5e6d633805d2d323e6fd81b4684b93a2677d45e7421c2c6aea259b855a698fd7d13477a1fe53e5a4a6197dbec5ce95f505b520bcd9570c4a8265a7e01f89c0c002c59bfec6cd4a5c109258953ee5ee70cd577ee217af21fa70178f0946c9bf6ca8751793479f6b537737e40b6ed28511d8a2d7e73eb75f8daac912ff906e0ab955b083bac45a8e5e9b744c8506f37e9b4e749a184b30f43eb188d855f1b70d71ff3e50c537ac1b0f8974f0fe1a6ad295ba42f6aec74d123a7abedde6e2c0711cab36be5acb1a5a11a4b1db08ba6982efccd716929a7741cfc63aa4435e0b69a9063e880795c3dc5ef3272e11c497a91acf699fefee206227a44c9fb359fd56ac0a9a75a743cff6862f17d7259ab075216c0699511643b6439"
			== test2.finalize().toString());


	// my own tests
	REQUIRE(Hash(512) == Hash(512));
	REQUIRE(Hash(512).addData("123").finalize() != Hash(512).finalize());

	REQUIRE("7f9c2ba4e88f827d616045507605853ed73b8093f6efbc88eb1a6eacfa66ef263cb1eea988004b93103cfb0aeefd2a686e01fa4a58e8a3639ca8a1e3f9ae57e235b8cc873c23dc62b8d260169afa2f75ab916a58d974918835d25e6a435085b2badfd6dfaac359a5efbb7bcc4b59d538df9a04302e10c8bc1cbf1a0b3a5120ea17cda7cfad765f5623474d368ccca8af0007cd9f5e4c849f167a580b14aabdefaee7eef47cb0fca9767be1fda69419dfb927e9df07348b196691abaeb580b32def58538b8d23f87732ea63b02b4fa0f4873360e2841928cd60dd4cee8cc0d4c922a96188d032675c8ac850933c7aff1533b94c834adbb69c6115bad4692d8619f90b0cdf8a7b9c264029ac185b70b83f2801f2f4b3f70c593ea3aeeb613a7f1b1de33fd75081f592305f2e4526edc09631b10958f464d889f31ba010250fda7f1368ec2967fc84ef2ae9aff268e0b1700affc6820b523a3d917135f2dff2ee06bfe72b3124721d4a26c04e53a75e30e73a7a9c4a95d91c55d495e9f51dd0b5e9d83c6d5e8ce803aa62b8d654db53d09b8dcff273cdfeb573fad8bcd45578bec2e770d01efde86e721a3f7c6cce275dabe6e2143f1af18da7efddc4c7b70b5e345db93cc936bea323491ccb38a388f546a9ff00dd4e1300b9b2153d2041d205b443e41b45a653f2a5c4492c1add544512dda2529833462b71a41a45be97290b6f"
			== Hash(512).finalize().toString());

	const char s3[] = "a";
	Hash test3(512);
	for (long int i = 0 ; i < 72; ++i)
		test3.addData(s3, strlen(s3));
	REQUIRE("7bfe6348bd840ae789325524a6865557bf4b3d30f1f895f8d5210e79ed82e2c59130e38b78e3db0bd46ca2babc7d688c8340624f38477df62982591f0dc929582bc153c75aa06df7f182cf278b63f7667d8b967281b03fd1cf6ad4c6261b3c33f120dbfe5768680eb5e47fea8bd41026bf004bf7453b4739a73d8b984b7e4a2f186e3149d065752c32f075f4907ea12d3cc1c6fff3c78d53adca00825e400c7be807cecf2141ac80dd3afb2d4011cb0e04dbd3e568f8fb6242b6b232ea174da9be7d2f85952d49c7c9d07befb003988a7d7db0abec1dbb5876bb51e782feb68e22feb0638fd8b60996f90a737bee649728eb4df376a846b550992f357c17018c1ca10a4224b77d72530bdea3b85e2664d21b6e5a7e1076ba508e95e2f8a9fce147aaa970e983cf2e98123d5c08e068fdcd2c46c13875a060eb71a2ecf622f68fe9d8aacc01d139674b8a89e727a9c8b1226391e41ec248d571d10035ffe1c66cc62ed28bcd242782e6ec4c7f790b863b6585aa25676307fa1d66cb2ba313edc1b55ccbfbbba0be6a6ead635f12bc93cf9a932f459b7d9a90cf96729590cb4ebd381cf03ee855762f536fd684854a28d68f5344d8a6265bff31e75f23faf254470574fd97528c58c50ac0be48ca7d6b22ada85b66b3d209c40070978ba0ec152e3042b703e47e3a93822f6a167fe42d6e7acba9e573f5e84cb12feda9399e4bf2"
			== test3.finalize().toString());

	const char s11[] = "The quick brown fox jumps over the lazy dog";
	REQUIRE("f4202e3c5852f9182a0430fd8144f0a74b95e7417ecae17db0f8cfeed0e3e66eb5585ec6f86021cacf272c798bcf97d368b886b18fec3a571f096086a523717a3732d50db2b0b7998b4117ae66a761ccf1847a1616f4c07d5178d0d965f9feba351420f8bfb6f5ab9a0cb102568eabf3dfa4e22279f8082dce8143eb78235a1a54914ab71abb07f2f3648468370b9fbb071e074f1c030a4030225f40c39480339f3dc71d0f04f71326de1381674cc89e259e219927fae8ea2799a03da862a55afafe670957a2af3318d919d0a3358f3b891236d6a8e8d19999d1076b529968faefbd880d77bb300829dca87e9c8e4c28e0800ff37490a5bd8c36c0b0bdb2701a5d58d03378b9dbd384389e3ef0fd4003b08998fd3f32fe1a0810fc0eccaad94bca8dd83b34559c333f0b16dfc2896ed87b30ba14c81f87cd8b4bb6317db89b0e7e94c0616f9a665fba5b0e6fb3549c9d7b68e66d08a86eb2faec05cc462a771806b93cc38b0a4feb9935c6c8945da6a589891ba5ee99753cfdd38e1abc7147fd74b7c7d1ce0609b6680a2e18888d84949b6e6cf6a2aa4113535aaee079459e3f257b569a9450523c41f5b5ba4b79b3ba5949140a74bb048de0657d04954bdd71dae76f61e2a1f88aecb91cfa5b36c1bf3350a798dc4dcf48628effe3a0c5340c756bd922f78d0e36ef7df12ce78c179cc721ad087e15ea496bf5f60b21b5822d"
			== Hash(512).addData(s11, strlen(s11)).finalize().toString());

	const char s12[] = "The quick brown fox jumps over the lazy dog.";
	REQUIRE("634069e6b13c3af64c57f05babf5911b6acf1d309b9624fc92b0c0bd9f27f5386331af1672c94b194ce623030744b31e848b7309ee7182c4319a1f67f8644d2034039832313286eb06af2e3fa8d3caa89c72638f9d1b26151d904ed006bd9ae7688f99f57d4195c5cee9eb51508c49169df4c5ee6588e458a69fdc78782155550ef567e503b355d906417cb85e30e7156e53af8be5b0858955c46e21e6fa777b7e351c8dba47949f33b00deef231afc3b861aaf543a8a3db940f8309d1facd1f684ac021c61432dba58fa4a2a5148fd0edc6e6987d9783850e3f7c517986d87525f6e9856987e669ef38e0b3b7996c8777d657d4aac1885b8f2cfeed70e645c869f32d31945565cb2a7d981958d393f8005dbffb0c00dfccc8f0d6111729f3a64e69d2fd4399de6c11635a6ae46daa3e918d473c4e0b2bb974c1ac393977306759ea3989109a45df35c6783b18702d468e63628d3923758274e0101a9ebf81b36c5554864d5f05d66689991e33258d38b7dc43acb1243276714f2a2c73cdc07b4da024e0310d618bc9be178ba2a86f94c08951731446841b05f0d91550b2bc085466922c621b39fd9ae817f14c1b77409ac4273065807a8b18385d712d2ca0893df1bc13b5bd89525dd4d068b88d8e888ee83d4fcb9bd3fa62ac4933aaa1eb7878c74d8069d1d0c3899c65a574d2be7b3e30f5fbf8665ef12457a5e05cee884c"
			== Hash(512).addData(s12, strlen(s12)).finalize().toString());

	const char s13[] = "The quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dogThe quick brown fox jumps over the lazy dog";
	REQUIRE("9c14ca7945d12a1d049cd18c82fb80d58c02c70709422f2f1b726ca270736b4e3c64dacc5c9cf729aa45a97dd1b4577493eb745e6b290d4800cab225f81d9d1f5fe9ab8ce3b901bb3a2f88697aae63ac3b20b7e6c5599d14d3837240a44dad3403f4758dffe99da8c97346eea2c39f236e26a47b4f03016129de1e8ff2d8fa008f12ee2203fb2bfd483d44a10707e5c53dbe7e843104ff5b2ef090e928ab66d686f54e37c9cd0eaa9f792d9c41d6b55054eff615b0f0dc186a1ee10ac710bf13359ee7237efe97bde9b38f9ef9e1f029081711a0fd1a0f4f14b71fe28fa8ececc1eca91596dfdc041806246df6b4be25749322b6912a9eb3d8dc9c67ea7e0e82fcc0ce63fddb8c3c54c9a30d212668dc8c0464765c6eaf89c47d763bea9df52ee95b1ff83007a0361a96ea4834664a9d70c42d839472508248411403dea856d7355e2ae43ae3732a8e05aafe786fa2c65db345ba15c10ce39e7254f931a405428d5910b87b382a06941d9691ead6aa417c6264252f4c22bb1dffe1a7c55085a75499057d780772c3fec3be8ac3f26cfd7198039257bab22e7af817d80d1ed694e757012ad6baf1de95895466fca7c899b554fcb1820653832a2aa08684b2b07d4645f2657827ca394e8f2fc1072d06d15abca1c72f6b19d31b3b055d109b2e11ab7049abc2b7f3871bad48bbf3443c5da1368395e86a5485b53bcfe568af3164"
			== Hash(512).addData(s13, strlen(s13)).finalize().toString());

	const std::vector<char> s14(73, 'a');
	REQUIRE("7bfe6348bd840ae789325524a6865557bf4b3d30f1f895f8d5210e79ed82e2c59130e38b78e3db0bd46ca2babc7d688c8340624f38477df62982591f0dc929582bc153c75aa06df7f182cf278b63f7667d8b967281b03fd1cf6ad4c6261b3c33f120dbfe5768680eb5e47fea8bd41026bf004bf7453b4739a73d8b984b7e4a2f186e3149d065752c32f075f4907ea12d3cc1c6fff3c78d53adca00825e400c7be807cecf2141ac80dd3afb2d4011cb0e04dbd3e568f8fb6242b6b232ea174da9be7d2f85952d49c7c9d07befb003988a7d7db0abec1dbb5876bb51e782feb68e22feb0638fd8b60996f90a737bee649728eb4df376a846b550992f357c17018c1ca10a4224b77d72530bdea3b85e2664d21b6e5a7e1076ba508e95e2f8a9fce147aaa970e983cf2e98123d5c08e068fdcd2c46c13875a060eb71a2ecf622f68fe9d8aacc01d139674b8a89e727a9c8b1226391e41ec248d571d10035ffe1c66cc62ed28bcd242782e6ec4c7f790b863b6585aa25676307fa1d66cb2ba313edc1b55ccbfbbba0be6a6ead635f12bc93cf9a932f459b7d9a90cf96729590cb4ebd381cf03ee855762f536fd684854a28d68f5344d8a6265bff31e75f23faf254470574fd97528c58c50ac0be48ca7d6b22ada85b66b3d209c40070978ba0ec152e3042b703e47e3a93822f6a167fe42d6e7acba9e573f5e84cb12feda9399e4bf2"
			== Hash(512).addData(s14.data() + 1, s14.size() - 1).finalize().toString());

	const int s15[2] = {0};
	const char s15_2[8] = {0};
	REQUIRE(Hash(512).addData(Hash::Span<const int>(s15)).finalize().toString()
			== Hash(512).addData(s15_2).finalize().toString());

	const unsigned char s16[] = {0x00, 0x0A};
	const auto s16_1 = Hash(512).addData(s16, 2).finalize().toVector();
	const auto s16_2 = Hash(512).addData(s16).finalize().toVector();
	REQUIRE(s16_1 == s16_2);

	REQUIRE(0x7f9c2ba4e88f827d == std::hash<Hash> {}(Hash(512).finalize()));

	Hash h17(512);
	Hash h17_2(512);
	h17 = h17_2;
	REQUIRE(h17.finalize().toVector() == h17_2.finalize().toVector());
	h17 = std::move(h17_2);
	REQUIRE(h17.finalize().toVector() == Hash(512).finalize().toVector());

	Hash h18(512);
	Hash::BaseType h18_2(512);
	h18 = h18_2;
	REQUIRE(h18.finalize().toVector() == h18_2.finalize().toVector());
	h18 = std::move(h18_2);
	REQUIRE(h18.finalize().toVector() == Hash::BaseType(512).finalize().toVector());
}
