#include "hash.hpp"

// ---------------------------------------------
// Hash
// ---------------------------------------------

// [ Methods ]

Hash::Hash() {
	throw Exception::E_Hash_Init_E;
}

const uint64_t Hash::generatePosKey(Board * pos) {
	uint64_t finalKey = 0;
	int32_t piece;

	for(int32_t sq = 0; sq < 120; ++sq) {
		piece = pos->pieces[sq];
		if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
			ASSERT(piece >= wP && piece <= bK);
			finalKey ^= PieceKeys[piece][sq];
		}
	}

	if(pos->side == WHITE) {
		finalKey ^= SideKey;
	}
	if(pos->enPas != NO_SQ) {
		ASSERT(pos->enPas >= 0 && pos->enPas < 120);
		finalKey ^= PieceKeys[EMPTY][pos->enPas];
	}

	ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);
	finalKey ^= CastleKeys[pos->castlePerm];
	return finalKey;
}

// ---------------------------------------------
// Hash - Constant Values
// ---------------------------------------------

const uint64_t Hash::PieceKeys[13][120] = {
	0x790f7f3ffbeb4567ull, 0xdbf5777f5b6d5cffull, 0xfbff787fffdfd8baull, 0x97feb8bff57fe146ull, 
	0xec75bffbf6f6231bull, 0xfb7f7cdeffd7255aull, 0xdbbefffdc3efc4c9ull, 0xbf34ff96df1bd7b7ull, 
	0xbceefd4747eee125ull, 0xbffdfbb5ef3f5ae9ull, 0xd7f69da7f373d0cdull, 0xbfae877beeaf8611ull, 
	0x5edeff7fdefff521ull, 0x585bfd2b651fdde9ull, 0xefe1228fd5bbadfcull, 0x7fef72b7dcfd2a97ull, 
	0xcfd55e9f5a1fd78full, 0xffd6cfbfea5dfcb0ull, 0xb76ba7ff7fef8d3cull, 0xbf1fdb7ffec5a45cull, 
	0xfe6f7ffabfff40fbull, 0xdfd2affff6eea529ull, 0xefff1bfd67dbb77cull, 0xbdfbfbfee75fa861ull, 
	0xffdaad93df7cb6a8ull, 0xffc3bfedf7feb5ebull, 0xbff42fff7fa70bf7ull, 0x397d7c07dff70ae5ull, 
	0xf3fffddf89abb148ull, 0xfff3f7af1fbf5d23ull, 0xaffffadf7ffbc564ull, 0xfeefacfbdaaf82c5ull, 
	0x71edfbdb6ffa813bull, 0xfc17fef71aa7f09eull, 0xf42217a7effdd9acull, 0xcddcfbf3ff1ff03bull, 
	0xf2abb3dfeff7d648ull, 0xfcbfe1feced1016full, 0xb7fe3deadbfab07dull, 0xffdfbf5bdfbf811cull, 
	0xdeea6f9eff5e18f8ull, 0x3adf7efe7fa72acaull, 0xdfadbfdff7799938ull, 0xf76fdf9feedf1a34ull, 
	0xdff7abbf3de5c5faull, 0xb3bffe8afff6d486ull, 0x5fcf7ffffddf6a55ull, 0xf7efd6538bfde373ull, 
	0x3f7dfeecfe7fff36ull, 0xad6fe7eff7fffcf0ull, 0xffbbfaff5ee78d15ull, 0xfffaafdc5546b5a9ull, 
	0xf6ef7bd77df4858aull, 0xde33d7fffb7fe80aull, 0xfb7f7faf7e3e47a8ull, 0xbffff7f7eff6ab8eull, 
	0xff79fcfbfdff9d09ull, 0xfdf7fdfb7fdfa233ull, 0x6cdafbebef979e7full, 0xd783f7adf76f6e47ull, 
	0xdd5ffbee6fef8c4aull, 0x78ffeb7bdbc2e823ull, 0xbfffbffd3bdb4807ull, 0x7573eebeff9328b9ull, 
	0xbfbfee7e6f47f63eull, 0x6bdbbefdef9de867ull, 0x9eff6fd57d79b9a3ull, 0xfff53d3faffbda34ull, 
	0xf773febd93df8110ull, 0x57ffb57d6cff0401ull, 0x7dfd73ff5bff6e78ull, 0x9f3f7bc7bfe97e23ull, 
	0x7aafdf57dfff256aull, 0xfc7ffefffdfbcf25ull, 0xbf672cbff2eb413aull, 0xfbabdbdfff1f1690ull, 
	0xdffdbffe675ef49bull, 0xeb7d777f7dac8ecfull, 0x7bf57bde66a9ac5eull, 0xfeffdfeb76fe8ab2ull, 
	0x779eadbe9fbf89d8ull, 0x9f7b7bff6ff34a82ull, 0xb7ce3dbddf3b9a87ull, 0x97f9ad7cfbbfa88aull, 
	0xbbf4ef7f7def7044ull, 0xdebbff5e756be74eull, 0xbe57f5eff7f6bdadull, 0x7799bffd5ecb88ecull, 
	0xc7ff6f5beda72b8cull, 0xcddc4eeef7bf97c0ull, 0xff6eb7fedf3745e4ull, 0x7dff91bbfef7e063ull, 
	0xffd7f7dbd7fee6b4ull, 0xfb736edd83d7c3afull, 0x7bfffdaf77dfb207ull, 0xf91dfdebf7d68e34ull, 
	0xd66cfffd9b6bd089ull, 0x7b5edbb908fb93c1ull, 0x7d7adffbd77c4a67ull, 0xa5f5f9ae7ab7e7ecull, 
	0xffffffffea9f45e5ull, 0xd796f7fd7b68e509ull, 0x79f57bed5ddfd6f4ull, 0xbfd7fe9af7be8fcaull, 
	0xdff77ffafe9f28cdull, 0xb85ee8fdff7b7c7aull, 0xff9efeaea5eeeccaull, 0x975bf7dffeff9f2aull, 
	0xfbfbfbf1cce7afc9ull, 0x7dfdbcffb73c2503ull, 0xb7dedf7b4e6fbf6aull, 0xce77e9df7dbe7c99ull, 
	0xc6bf9b2effbfc322ull, 0xdffbb7fbea7fa5d1ull, 0x75b9ece4efd5be45ull, 0xdd9f6adfff6681ffull, 
	0xff7df7fbfa23cb01ull, 0xb5efffd972ef8102ull, 0xff5fdddffddfb662ull, 0xfcaebdff7ae78581ull, 
	0x3ebfffba6ffd843eull, 0xfe5ee7dfd7ff284bull, 0x67bffebadfbb564full, 0xf7de3ffadecdb1ffull, 
	0xf5eeef6ef7f9d1feull, 0xeb2fffb7df7edd0dull, 0xff6ffbfeeeff8d32ull, 0xf9f69ff6f9f7f56aull, 
	0x7c3fb6feb3fa1002ull, 0xf8bc7f7fdff69816ull, 0xffff77e66fce91f0ull, 0xffffebb6f66ddfcfull, 
	0xffdf3ef535f7b29full, 0xfed7fdbeef5bcf61ull, 0xfd9ff5e6cfdffb0dull, 0x7fb26fbef7eed60eull, 
	0xdaffffb7d6f995e5ull, 0x14d1eacdfffffa3eull, 0xfef5d4fff996a02eull, 0xfdb5f173e7fbe6b9ull, 
	0x7db5de37f7ffe770ull, 0xbff5e7ded9bf3c89ull, 0xdafb0ff75fc3c301ull, 0x7f9f67b5ff0f328bull, 
	0x36b6ebfefb67aee3ull, 0xdeedebbfffade198ull, 0xf37efbdfa7f8e255ull, 0xffbdffbfeffe62c2ull, 
	0xfe6ffdf9dff79613ull, 0xf9ffefffeaddb4e2ull, 0xbb4ef7d77e561a81ull, 0xdfa6f1bfd5ffb7eeull, 
	0xdefd78efab7fc93bull, 0xdf9f7f5fedb55dfbull, 0xeb2df597fd9f0446ull, 0xf2bdf6fddeaed80eull, 
	0xbfaf7ffe9876261full, 0xfb9f6daedfd78454ull, 0xffbafff1effb39f8ull, 0x5fdb7d87fdecb973ull, 
	0xffddf2eabfd7f860ull, 0x9b8af7d6dffa9031ull, 0xd9d8ff43d73fb70dull, 0xf7efbdfbed7de7cdull, 
	0x73b7bfff77bf35d5ull, 0xf7ffffe7f3e7b5eaull, 0xfff6ed7fcaadb59cull, 0xbbfeb16e7fff805aull, 
	0x9fffbfff7bfd4250ull, 0x35ff4d797f95655aull, 0x7faefbefdfdf8ffbull, 0xf7a6c51aff98d29aull, 
	0xffeb77dff7e74e4eull, 0xbd06ff97ffffba71ull, 0xfb45fe3bf5fde228ull, 0xfcfff3bfdefa9237ull, 
	0xffdebff6efffee9cull, 0xde9fa1f7dff5cc6dull, 0xb5ffb7f3f7cfd273ull, 0xf787bf8cf7af2dc0ull, 
	0xbf7f4efbeb1b0a9cull, 0xfdb7aefee699dcbcull, 0x79ebeafdde6fcb3dull, 0xd4dfebf7aff77ed4ull, 
	0x1ba669ef7e4f80c8ull, 0x73ef3ffffbed5530ull, 0xfc696ceda6ff9756ull, 0x5c5ffd7aef7ff671ull, 
	0xb3fbf7e5eff5e9a8ull, 0xfffefbaddfffca09ull, 0xfddf7c7ffdcfa63bull, 0xdfffbf7bd771786eull, 
	0x7dff2cffff7f42e2ull, 0xee6afff8fdf5bc47ull, 0x8fb7ebd07ddea498ull, 0xfff939f3e7d4c122ull, 
	0xffdf77defbd7fca2ull, 0x9f7fe34fff7fd049ull, 0xdff7ffbffe37aa96ull, 0xdffdff3fdfef8dcbull, 
	0x7ff2eafbfba7d445ull, 0x1f71d5f35ffff83full, 0xd7d7f9dd7e57098aull, 0xd59ffffebff7a76aull, 
	0xf9bf7f3dfb8bfc02ull, 0xfbd37ff9effbcb8cull, 0x7da7ddfbfd72bbf5ull, 0xdfcfd7fdfffde690ull, 
	0xfefffd56ebd68302ull, 0x02d5fd5eff7ad6cdull, 0xd17f7d9df2edd574ull, 0xbfedf9dbbeb427a1ull, 
	0xd7677f7fbddfe48full, 0xb78fbfed19e7bfccull, 0xef77f9ffffff2b79ull, 0xff67fbedcd3f95deull, 
	0x9b75c5fefedee918ull, 0x7931affc419fe0ddull, 0xbf7f2b5dffdfb154ull, 0xd79bf3e6edffc4aeull, 
	0xf946bbb471ffa284ull, 0xfe7fee7ffffda596ull, 0x7ffeeef77fffc87dull, 0xdffeeefff1fb7e02ull, 
	0xbddffbf9f7fd357aull, 0x5bddf9ffd7dfc834ull, 0xd777f7f77ff9f4c7ull, 0xfe987dfff4eee2e4ull, 
	0xffdb7f3cffa33267ull, 0xfc7fff7eeec9e8c4ull, 0xffdbbbbfeefdbf76ull, 0xf79cfffb779ef360ull, 
	0x7aefdbce76e6c3cdull, 0xb6ffdf9df76fde99ull, 0xd9effefc7f2f9e9dull, 0xd2fe6fb89e9d98b7ull, 
	0x7cb6effffeed1c37ull, 0xdff6fffffd6ff1c8ull, 0xd5fbbffffcfba59cull, 0xfeee3efff7b7a7ffull, 
	0xdfbbe2fff87f8a0dull, 0xfe7eb71fff5bd013ull, 0x3fe77fff577df487ull, 0xea7c3fdf4de4a782ull, 
	0xbf97e3e27cefddd5ull, 0xf7f56d67f3dfabefull, 0xfbbe7fefe8dc526bull, 0x9d9df9ead77aa7fbull, 
	0x65aeef775ffd3aa3ull, 0xfdb7ffafeeffa137ull, 0xbfefeec5fddb6c3aull, 0xdbfabb755b9d5ebfull, 
	0xff7deff37f1e63f1ull, 0xffadfbaefdf5c2a3ull, 0xffdb7bebd179893dull, 0xcf56ed77effca7faull, 
	0xbbdcbfffe777f590ull, 0xffed6a77ffff7940ull, 0x37f77ffe5f9d83abull, 0xb58de76bf7ff1dcfull, 
	0x3cdf7b7f68df9da3ull, 0xaf5ba7fef7ffb582ull, 0xdfef5a1764bfe573ull, 0x6dfb7f5dffff4674ull, 
	0xfbe3fc1ff9bfb9abull, 0xfbfefd7b7f75d608ull, 0x777eb4bffedcbeb4ull, 0xffbbbee5f4bb991eull, 
	0xfeef7b276f6ea3e1ull, 0xffdcf7ffbd7fbbadull, 0xe77caf7fdaef9a9full, 0xfbfbfe77cdf6208cull, 
	0xbbce9b66f9f74123ull, 0x7e99f97fdff7d17dull, 0xbfb762fef3fb0d1eull, 0xffbfbfec5bbc17cfull, 
	0xffffffdedbef911aull, 0xfd93e5bbdfe2aa44ull, 0xfcfd6b7b7efff30eull, 0xb11deb9fd9d3a800ull, 
	0xbabbf78ccfae8ec3ull, 0x3e5bf54ef4f72a23ull, 0xfecebfbf5bf772bbull, 0x7e7b1f3f7f7f6edcull, 
	0xf3ed7d7f73cf6507ull, 0xffffbfdbfebf94feull, 0x3dfc37df6f7eed90ull, 0xd53c97cefbe76430ull, 
	0xbb9f3727ef7fcf1bull, 0xdffbfaafddfa996eull, 0xdf3614ccf77b92ffull, 0xffbfffef5ef7d652ull, 
	0xfdfff7ff1df79013ull, 0xbeff3faf66ffab37ull, 0xbff9f7f9cdfd9346ull, 0xeeff72effefdfc58ull, 
	0xfd979fff7df7b714ull, 0x3ffffb5eeffd4a46ull, 0xf77d7dff7f3ddd67ull, 0xa76737ee6a612e62ull, 
	0xbf2fefffdffb4b45ull, 0x7fff93d7abfedba0ull, 0x5beffdbacebe2e75ull, 0xef7f7cfff7d9d91bull, 
	0xf2f3dbff7f697021ull, 0xff3d97ecf3feff53ull, 0xfd103fffd1e89c03ull, 0xbf3effce5bd7c986ull, 
	0x78fe3fcbe3ae8fa8ull, 0xfe15f69e8fbfec9aull, 0x7f7edbdef6ff4d2dull, 0xd8757fb95deeaef8ull, 
	0xdfd7efffef45af4eull, 0xdafeee8f7c1fb4e0ull, 0xdfffe2ffdbd77406ull, 0xfef6ffdf7be2ec3cull, 
	0x6beb7efee9ffd5b0ull, 0xedffefdffbed313dull, 0xfddde5dbdf7fceecull, 0x3cbfefffddfd1bb2ull, 
	0x17ebfff7bff7a6d2ull, 0xbfb5ff7ddfd63898ull, 0xb5cbfd97d7afd524ull, 0x9eb3f59f5fafd5f5ull, 
	0xd3d7f37dfffe7b17ull, 0xfebfea7fd5ffcb24ull, 0x7e777ffedfdf654eull, 0x5fbfdc9ffefb80c4ull, 
	0xb97fff6d6b5ff4b2ull, 0xeffbe97ff7e78a20ull, 0xbffe66f6eefe4f0eull, 0x7b6b7779fbfbf85full, 
	0x7cedfdb77fd6d7b0ull, 0x5b3b6fafef5bb751ull, 0xd517f529f73de6faull, 0xcabebefefdfff55bull, 
	0x7fb7f9ea7e4fabe9ull, 0xfedefecbdf3f9d13ull, 0x975f3627ff5e41dbull, 0x3ff53fcff4f78ff3ull, 
	0xff8f34ff57c243f1ull, 0xdcf7bff5f7e7eb1aull, 0x77fe1dff7afb746full, 0x96df27bfebd1c94full, 
	0xfef9beef7e7b5565ull, 0xffefecf7dbffb044ull, 0xeceffbf5ff72936aull, 0xefdf673ed829fdb8ull, 
	0xbfffabe57fdd8000ull, 0xdfebff5febfdd568ull, 0x3c6e7f9fffddf0f7ull, 0xffd7faede9fc5ea6ull, 
	0xf7e7efd45d9f08b8ull, 0xbfffafffdbfb8b27ull, 0xf67eb6f7dffdad2full, 0xf7ffebdd57ff3e98ull, 
	0xbfffefdfb777c4d6ull, 0x7f77e7eefddfe87eull, 0xfffffbdfffff6585ull, 0xfdf1fd8cb77ef3dbull, 
	0x57f71f773ddf92d3ull, 0xfdaf9d7fffa66c56ull, 0xd3aff32f7b1f792full, 0x7ffd6bfffaf6d84eull, 
	0xc63bbd34efbed516ull, 0xffafeb3b6aff963dull, 0xbfb376ef9f998108ull, 0x77ff3fceb5ffa634ull, 
	0xe2daeef57fff4a69ull, 0xfb70adfbff7bb8c2ull, 0xfc5bf12ebf5f9e8dull, 0xaffdbbfeeeedc37eull, 
	0x7f5efd3ff9fe1091ull, 0xb2ef363cf87ce04full, 0xd727fa7eeeffafa8ull, 0x577e7dfffb4002e8ull, 
	0xd5ccf3b8ebffbe2bull, 0x5afeffd73af3884dull, 0xd59ffb9b757b8892ull, 0xfdfbfdedb7bf94ceull, 
	0x77bfffeb8e666f90ull, 0xdfd2db41ddfb4f24ull, 0xf9fdf17732f77d8cull, 0xf77fdb3ebdfe9f92ull, 
	0xdb336f7dffbfe29dull, 0xbf7f7df3cfec2714ull, 0xdebfddf7feefea91ull, 0xf9bddffe7b7c7dc6ull, 
	0xfe646ef56bbdcbedull, 0xe7fffcbfdf5f92a4ull, 0xe77effffd99d0edeull, 0x3fe83a4fedada777ull, 
	0xfffbb7bbfff9d414ull, 0x7affff5bbeef7944ull, 0xbffcafefd7f4a051ull, 0x7fbc7fffefe391c4ull, 
	0x66fffc17ddf0a8fcull, 0xfeb7fc67ffbf6ba1ull, 0x678c6f6bf5fb45b2ull, 0xf6ffffd1dfbbcd90ull, 
	0xddadebd3aba7bda8ull, 0xf5f7fcfde9f63e34ull, 0xda1ff2adee778fdaull, 0xfe50ef3cffeff166ull, 
	0xdbf9bff9d63fb0c2ull, 0xaf8df66f6dfbfe0eull, 0xffdf68d8dcfbaf1eull, 0xf2d57fbffbdf9453ull, 
	0xfbddbdffbbf71379ull, 0x77f7fc7fefffbba3ull, 0xf57ff7eb783c9288ull, 0xdfd856fed5ff9684ull, 
	0xf9fefc5f6ff20264ull, 0x72edaf9fffefbaa2ull, 0x27ff6cf5cdffd8b5ull, 0xfffebf4fffaf99e2ull, 
	0xdf8e7bffef2efa14ull, 0xfd3bfbbffd6f6145ull, 0xaf7bbff9fefd6a98ull, 0xfef6ffe677ffa230ull, 
	0xfb3f67d6feefe57cull, 0xf97ffdfdffda8002ull, 0x7f5defd7713b2f64ull, 0xffffa84ffe7d97a5ull, 
	0x67d725d2f7f5cc41ull, 0xfff3ffbbeffd4202ull, 0xfdfcf9aef7ff6391ull, 0xfeabfefdf77b5bb8ull, 
	0xfdfa7d6fffbdaa7bull, 0xe99de5abfffc8bb4ull, 0x75fafb7dfffffc95ull, 0xdfd7fe2bfceff359ull, 
	0xbffffe7f26e7f21dull, 0xf5ffa8ef5cefd1e8ull, 0xad7bea041ffe56a1ull, 0xdf1fb7ccefcdda6eull, 
	0xbf95dffd76c91a9bull, 0x79ef7756f7f53d88ull, 0xff6cfbffbcbba7b7ull, 0x54acdfeff7d5e0a9ull, 
	0x3fdfef3f7ff9fe5aull, 0xffed7fdf7f6ca831ull, 0xb49eefdff0fee2d6ull, 0xbcb7b7deefbbb3d2ull, 
	0xfb7fbf277f6fa89aull, 0xbfffdf4efdbfa9caull, 0xbecdac5fd7fdadcfull, 0xf7ffbfdaffbeb720ull, 
	0x7fff6df67c37c6f9ull, 0xfebae7fff9bf6663ull, 0x3ffd7717febafb2bull, 0xda7bb7cef6dd7623ull, 
	0x5cfa77be4b9614b5ull, 0xfe1ee57f6ffff343ull, 0x6ef5aafffbf73bc8ull, 0xbf75bdfdfd2b62c6ull, 
	0xffeacf3bfbee1b3eull, 0xfb31f3ebfefeea4dull, 0xfdddf87fff9ff748ull, 0xb1eef7efdcddf974ull, 
	0xbffbf8f7e7f5f12aull, 0xdffeeff3fff2170cull, 0x7a7fffdfbfe017a9ull, 0x9bffdfeb5fa7b8ffull, 
	0x7ffbfdbf7fbbbfdbull, 0x9ddaff9ffdbfe954ull, 0xf17fd577dd672824ull, 0xe3bff5e9d77f64a5ull, 
	0x67debeefde8eca8full, 0xf9ef19dd77f9cb50ull, 0xfea4fff5773e4f1eull, 0xfffaf9f7ef7eba4cull, 
	0xfcfdba53f7d7920eull, 0x1d7deab76f4f766dull, 0x7e7dc7ffcdffaa11ull, 0xfff2fffed7ffa831ull, 
	0x96f6aaee9f5e3200ull, 0xfffef4ff7afd0fb9ull, 0xf37ffedd76b3f990ull, 0xddfb7fff78efe52eull, 
	0xfedfbbafedeac79eull, 0x7e97bf7b6f77b9acull, 0x2fbcafdedf46a636ull, 0x6df67d137dcdb0e1ull, 
	0xb9b65e7e1fdb4a6dull, 0x7fd8fa77fdfe8ae7ull, 0xf74aefdbdcd9439bull, 0xffdf3d77778131e9ull, 
	0x9d2f7fd74b722441ull, 0xd9a8fb97db5dd53full, 0x367daed7f7d9b90dull, 0x2f6feffcffef69a0ull, 
	0x61f7ff9dff7dda57ull, 0x9b7d7f7ef6efed52ull, 0xb3bff9175bf09502ull, 0xd7f5fbfdfaffd2d2ull, 
	0x5a6c7bdfba5d8053ull, 0x636f7bbbff87db85ull, 0xff36f9f77eed0b3eull, 0xfeaffd3be7fbad0cull, 
	0x3565afefffe3e418ull, 0xfbafe3fffdffb240ull, 0xffdef2bdefff7aa0ull, 0x93effaffffbf3186ull, 
	0x3bfbfff8f6df3a72ull, 0xbfffd599fe0fb200ull, 0x9de1fb877dffb48aull, 0xf77fbf9d6bf7b6bdull, 
	0xfbdfeece7baf544eull, 0xfefdf6f76ddd2afeull, 0xff79737bf9f54aa9ull, 0x93f57f6e5f2dcbc3ull, 
	0xb9ffdfdfad79ea22ull, 0x7fbfaadeeafddf58ull, 0xf7ffbf4779e5fe98ull, 0xffedfeedf2dbab63ull, 
	0xdbfbbfbddffa8765ull, 0x6f3deadedfde3d63ull, 0xfbdfa3ff7fbf4c9cull, 0x59d77fd7575e2d28ull, 
	0xedc2dddfe76f3700ull, 0x7fb7fadbebbb87e8ull, 0xae7fffbe7923e6b9ull, 0xf373fed7fdcd5f98ull, 
	0xc4efd7e73bef9505ull, 0xdfb6faa6675f4159ull, 0xfdfbfff7f7d10e31ull, 0xbb6ffddfe6fdf01bull, 
	0xedffbfffe3ff88d0ull, 0x3bafbdbfeff1e5f2ull, 0xde1b4f6ceb2551b6ull, 0xea5ff993fd0f49efull, 
	0xfbf7f39cbe5ce314ull, 0xff7d7dffdeff2616ull, 0xd7f777e3f3ef69f2ull, 0x37dc6c3fff0fcfaaull, 
	0xb1fef7e7452ee220ull, 0x3b7f6abfafdd0552ull, 0xaf7fb77bfff7e5b4ull, 0xff7ffffbdbd7ef0eull, 
	0xb7c7f574eeee503bull, 0xf57bf5fefefe9adeull, 0xfffcff58fedfca4aull, 0xffc7bf9fef9de13dull, 
	0xdbe9f5e738f45684ull, 0x3367f7dbef7fd2e1ull, 0xff26fffffff5c795ull, 0xb7beffdefdff1aeeull, 
	0xdff57b9ffefcafbdull, 0xfe77fb5ce7af852dull, 0xb7b5b3dfe7f7d625ull, 0x6d3bffefe67bc977ull, 
	0xdcdef9fb7ede68f1ull, 0xbdfbefbee9f3ec6aull, 0xf83fedffcfdd1553ull, 0xf5aeffc6ff6ff813ull, 
	0xf565b5b5fdc582aaull, 0xbcf7ceedbbfe6df9ull, 0xe3f7bffebf5fb54cull, 0xbeffdfeffccbe0bcull, 
	0xe9fbe5737eefe74cull, 0x3c5def3337fd13c8ull, 0x4f77aa9bb1d7af2dull, 0xffff34777cdfd17aull, 
	0xee9f759efebfb688ull, 0xb3e7f5d97bff4bb7ull, 0xfbf8ef66ff6ac9b5ull, 0xbf7ee2cfffe5e6f6ull, 
	0x9fef6e7df5f7ade6ull, 0xcede68dff3f59fa6ull, 0xefbff5fecbfd67c7ull, 0x1eeeefebf4efa004ull, 
	0xfcfbafd5dfffaed8ull, 0xfe7ae3df67ed3121ull, 0xfff7fd5ff29fd4b3ull, 0x1bdddfd2d327f40eull, 
	0xeabe2fbfff3780e7ull, 0xfdbff9effafecb2eull, 0xb8fb6ae7febf9075ull, 0x7ef9f4b7fff7c144ull, 
	0xde7f03fb4ebfbd64ull, 0x77fdf67fff6fc1bfull, 0x7ff7bffffe5bd078ull, 0x87f6fe5dfffff181ull, 
	0x7f6d3cfbe7efd2c3ull, 0x9779c3d77e7fc7efull, 0xffbc6a1fbe5fb182ull, 0xd9afecf77f77c340ull, 
	0x579ebfeffefbad22ull, 0xc3faeff1fb4b340full, 0x7f44bc3ffbefae3full, 0xe497ddfa5d6b1e3cull, 
	0xb9da6f6feb17abfaull, 0xb77ffff767dbf80cull, 0x9ffffe7effdeb001ull, 0x79f7eeeff59fc51full, 
	0xfdfdee7ffdff5cf4ull, 0xfa7fefbfcffc1bd2ull, 0x9739f9bd7ff77afeull, 0x5777f3ef785f9996ull, 
	0xfffb68e36f95e79full, 0x7dccff5eb3ff9caeull, 0xffffbaffd7af2dedull, 0xbfef774bff3fd753ull, 
	0xfffdaffcbdbc35c4ull, 0xc6bfb766feffe9a1ull, 0xfd7ff77fdfee8a93ull, 0xbeffeafe767ea69eull, 
	0xfffbebdbe7ff948aull, 0x7abffeabeffefb34ull, 0xf3dffbc77b9bd89full, 0xf7d2f22f7ffaed83ull, 
	0xd5e57f7ffed7c9b6ull, 0xfd788b6ff676bf5aull, 0xdcefedbafbcfa349ull, 0xfffbf3dddf6fc330ull, 
	0xcdf7b7eddbffa29dull, 0xf266bf3feede49a6ull, 0x577bf5fdeffd51b3ull, 0xfbbdffa77dffa830ull, 
	0x7acf6fa7fe67af5aull, 0x9eaf6f7ddef7a36bull, 0x37ff6b6b6e47dee2ull, 0x767fff3ffff2e662ull, 
	0x5fc27f7ffa7b6c1aull, 0xffcdfbf2ef3cefb2ull, 0xffeb497ff7d600c5ull, 0xf2be3d7ffcffc26eull, 
	0xfebbb5aeefbdc836ull, 0xff5b25bf7d7f59edull, 0xd78ff7ffd3fda6b1ull, 0xf77befe5f6d2c83full, 
	0xdf7bfcddfb2f5c0dull, 0xb99f3cdff3e56fdaull, 0xfffffcfdceffd484ull, 0xf9b837dec2d73864ull, 
	0x16effdd67df94275ull, 0xffbfff65bd55bd3dull, 0x7ffb69ddfff5cef1ull, 0xfecde77ff777c414ull, 
	0xfed9f37f57fbd0dbull, 0x72f7f6b756f33fe3ull, 0x747d5be76feb99d1ull, 0xfaefbde7febbf5faull, 
	0x77e77efef9beb4c4ull, 0xf9beeff5ffefbb8full, 0xd6c3ff7bcdf5f321ull, 0xb5beee77fffe525bull, 
	0xfe795bf6ef4eb371ull, 0xd6f6f106df9fd08eull, 0x73ffdabf65f5add4ull, 0xdff77fbfffff3b62ull, 
	0x79dbeffbfeff5032ull, 0xb7d2a5f6cfdfcacbull, 0xd8ef3c9fbd2320d8ull, 0x7fbffe577effd92dull, 
	0x9f9afe4fbf9f8994ull, 0xc757dbf1fffdbc39ull, 0xfdffff7fc77daa8cull, 0x3b87bfffff5fba59ull, 
	0xeeafdbd7f9c9c2eaull, 0xbfffbb5edffec869ull, 0x3f5775fef9adb999ull, 0xdd7dfbffffffc1fdull, 
	0xdfb8ff9dfbbf9111ull, 0xbe775bfbfbf1cc37ull, 0xf69f09df330dc516ull, 0xffff87c57f7e9b7eull, 
	0x559bfffff6f595e9ull, 0xf3be7bf7b76d2dfbull, 0xdf75eedfd2fd2cafull, 0xdf6efdbfffef8574ull, 
	0xdba6adeaeef7e6f5ull, 0x17cf636fd7bf9228ull, 0xf7fefd7edd5b3eeeull, 0xf7fddbf672bcc52eull, 
	0xddfeefb1defb8538ull, 0x6cd77dfebe8fed4dull, 0xf7d7bc2f3bb7f408ull, 0xd6ffbfbb7ffd2d5dull, 
	0x9fe3e7fd7bf1ca1dull, 0xefdbfe36fb76ea33ull, 0xd7ffffe7dfe7894cull, 0x76ffbc9ece7fd3a3ull, 
	0x73ff7f1ed7feea5aull, 0xcf7bdcefffafcf71ull, 0xbedeb9dfeedfcbc0ull, 0x6e7efbe6fdd39168ull, 
	0xfefdcfdf7ddfd826ull, 0xedbffffbfcafa366ull, 0xbf6af6fdd7e69cc3ull, 0xfecbfeced6bf8168ull, 
	0xe9f3fbbeffefe267ull, 0xddb7fdfffcffd625ull, 0xdefd7dbf7ef35126ull, 0x7cb7ff37d5dbf8ffull, 
	0xf97fffdf7fbcfd4bull, 0xe9cb7fbff732daffull, 0xdfcbffcebaf53184ull, 0xddddffdfd7bf94fdull, 
	0x7e6f7cb77faecc08ull, 0xf1d9bdbff5bfc5adull, 0x5ceff7ff7fefe12full, 0x6fff6dffeebf9fccull, 
	0xfbfdf6c63adfd156ull, 0xd7d7fdfffbd35320ull, 0xf7bffffefc9f8b4aull, 0xfbbff77fffab5540ull, 
	0xfcdf21fff3b59993ull, 0xfdb5ed768ffece2cull, 0xfbf3dd7b9d3faf88ull, 0xb32f9da37ed9fa43ull, 
	0xf9f5bfda7e6fb801ull, 0xf5c9ffd67e7f549full, 0xefefe67ffff9e2b9ull, 0xdef7fe7fdb7fb289ull, 
	0x5fbbf6f6ffdac2eeull, 0x6ffabffddf6fcc08ull, 0x2d7cf734fffbd13eull, 0xf6fe7fffdc4bb6d8ull, 
	0xffffbdcdff7fb1b6ull, 0xf6fced6f7fdd1501ull, 0xfefff337fcb7acbeull, 0xd9b9b73f67fc7e77ull, 
	0xbf3e7fff5fee9d1dull, 0x9fbdbfbf7f6e9216ull, 0x77f3fedf6b7db4cdull, 0xd7ffefffee1b9a40ull, 
	0xefe4fbfffe9c6253ull, 0xffdedffff7fe9dfcull, 0x3d76defdfda78b1eull, 0xd7fcf4bfffde3ce9ull, 
	0x3b5fbebaf63f8775ull, 0x7ffe75cfeeffbf4cull, 0xf7ffdab77fdecdbbull, 0x9b74fffeafd53bc4ull, 
	0x7f1b7bfdefff7663ull, 0x7cf873bf76f60087ull, 0xd3feff75f5fc230aull, 0xb3fffbeafb9fc887ull, 
	0xbec76bd77ffff73full, 0x66f731befdeffab0ull, 0xff7fffffde875d63ull, 0x5efb73aae6bf91abull, 
	0xf83cf7bf7cdb4fd1ull, 0xde5ffb35e7ffe80bull, 0x7ba7f7fdeeff2fe5ull, 0xe1bc1d75ffff820eull, 
	0xffaf7ffe3f23b325ull, 0xfcafffef4fefb687ull, 0xdf3ebfbaccad8e7aull, 0x5df7f6f7fa7fec66ull, 
	0xf7877b7ffebac543ull, 0xfbbfcdbd7f5f97adull, 0xffff5fffdcf3bfc9ull, 0xf7fdf73f5ffebf95ull, 
	0xfc77f299efff81d3ull, 0x7ea529ffa6fd9451ull, 0x5b07e7fffab5aa71ull, 0xc77bdbfffff7f1a0ull, 
	0xbfd7f7fef9cf96e2ull, 0x791fff3ff3fbaba9ull, 0xe09adbbfef317e1full, 0xff7efff74e7ba4a7ull, 
	0xfdadbeffefef9217ull, 0x5ff927ff7de78bbeull, 0xf9dffab0fbbeab04ull, 0xf3bf67bfff5d7f8dull, 
	0xf5fd2bff6fdfbc9eull, 0x757abff1eb7eb17full, 0xefedffdbffb4a444ull, 0x7ffafffd779fbf5aull, 
	0xfdfb7edcedbdbec8ull, 0x0bafbb7af9ff2eccull, 0xfdee7e37f2f78269ull, 0x374fedffff7e82f5ull, 
	0xfbdb67def7dffd33ull, 0xf49ff3d6fefb0441ull, 0xbfafbedff5fff512ull, 0xe7fedbec7f7bc430ull, 
	0xfdffffdd7f7df271ull, 0xbfdcdce7f7f71ca1ull, 0xfaf3d5b9ddfdb421ull, 0x77f7ffd76feffbd4ull, 
	0x7edfafbfc2e4e311ull, 0xf9bd6fdfcd74c318ull, 0x3bbffd7fdd4deec2ull, 0xf2cee6bbfbca45e4ull, 
	0x7f63acdf6f1aa269ull, 0xfadf9d5ffe721a57ull, 0xffef3395beb75171ull, 0xbff93df7f9b4edcaull, 
	0xffd87febf3998a4bull, 0xbdbc6fa6fbf75b40ull, 0xbf7778b7fa77cb60ull, 0xffffffa7ff637e80ull, 
	0x3ffcf776f3bcfe32ull, 0x5df7bafbf67e6971ull, 0xf7dd377dffe664bdull, 0xf9e3fd7dfb1e898cull, 
	0xb3bdbfefbfff2bdeull, 0xff6e8f7867b7fd51ull, 0xdc7b37affcdfc57eull, 0xdcf9ff7ef7fffa60ull, 
	0xe2b7ffcebfe6c809ull, 0x7bfff6f6cf9f0a96ull, 0x7eabffbefdd5bd46ull, 0xdb7fbdef1756eddbull, 
	0x46ff39efea3fe2b2ull, 0xd78ff13fd8dc9be7ull, 0x737b7bebf5359cbbull, 0x4de25def8eedb0b7ull, 
	0x5ef7e337f3bed074ull, 0x7cfd19d9fbd4e123ull, 0xbffdbff9bfedd180ull, 0xff9fff17ced7b445ull, 
	0x7c77ff7cface7598ull, 0xf5eda3e7f9ee33f7ull, 0xbbf7f1efffdfbec1ull, 0xbfefbdf5567dbdf5ull, 
	0xdaeb677ddbfc05daull, 0xdeffeff6ffefe35eull, 0xe53efcbd77f8cec7ull, 0x8ddffeb8ae572527ull, 
	0x7bed7ffdf6cffe29ull, 0x7dcbfdf9fbaeb041ull, 0xfafbf76fe5f7c31eull, 0x92dfffba5fb849a6ull, 
	0x7ff6ffeeebd6bc4dull, 0xddbdbed7d52de4a9ull, 0xdd9cefeef3cee0ebull, 0x7eff7fffca70dbb6ull, 
	0xd5eff8bffe6f8e90ull, 0xb97bdfef7ffeb4f0ull, 0xfdfcb69fffff9685ull, 0x65fe37ecebfd50bdull, 
	0xfebf66dbff5da6cfull, 0xdfffffef7d9cc1e8ull, 0x26e7ffdfbeab1474ull, 0xd56fffaffebda3aaull, 
	0xcd779b2374ff96e3ull, 0xaef768fffcffe766ull, 0xfef7fcad7ffbbc4cull, 0x3fabfffdcdf2dfa1ull, 
	0xf58dffeff9edd527ull, 0xf7eca77efd2aa02aull, 0xd7ddefd9f8ea82dfull, 0xee7edd5fab7dedafull, 
	0xafdbfff496f95a44ull, 0xbf75f7db963588acull, 0xfbcefee76caba2adull, 0x3ffcf7fb5f7fa48cull, 
	0xeabff6bdfebbc824ull, 0xcf3fe9ef79fb04b5ull, 0xdfdd76b7effddd0full, 0xfffbfb7e7faf9839ull, 
	0xda75fef4ed7c5672ull, 0xbc47bdcfbf5df54full, 0x7f6b5f3bd7fab015ull, 0xfbafebff7f7fd7d9ull, 
	0xdfbd77faff5f671aull, 0xdf58f5ffb9fd6bdfull, 0xbdb9bde5fffdb036ull, 0x7a7ff5efff0c9c2bull, 
	0xffc9ff9f35bb87c6ull, 0xbf8fbfebefbbf9fbull, 0xbb4fff8e7fd974ddull, 0x7febdff7df7fb71dull, 
	0xffa33fff7fe7b882ull, 0x3fb1fdcbfdedd4e9ull, 0x827bf7bffb399663ull, 0xfd6b7f37fd6dcca5ull, 
	0x3ed3d7f77fb99a6aull, 0xfff6cfcff6dbb819ull, 0x5ffabf7fffdd8ff5ull, 0xdf4eef4f3e5ff435ull, 
	0xdfdbbe67e8edffa6ull, 0x6bf9efddfdade240ull, 0xfdd3ecb5f8ff33bdull, 0xf9d4e5f7ff7fa177ull, 
	0xfbd7fb69eefd8291ull, 0xeff53febfff68974ull, 0xfeffffacdfd7be5bull, 0xb7deffd3fd1dffb2ull, 
	0xdfffeeeeffff0ba6ull, 0xbccfddcc77f30abcull, 0xfa37ff3f73bdb31cull, 0xf3eafdeb2fdad936ull, 
	0xe2ebb3befa74d1eeull, 0xdefdeef5ffdf90b8ull, 0xfedb7efef5f6d419ull, 0xffef52efa0f7ba38ull, 
	0xdff1fb47db5b5e28ull, 0xefef7f7fda7646d5ull, 0x9b6776db76f60536ull, 0x7ffd7b57ef6ddbc8ull, 
	0xda9eb5cffb7ed8f5ull, 0x7936f7fef7fe87e9ull, 0xf757eabf7febed11ull, 0x7ebbebf9fe7fb41eull, 
	0xff9feef7fbe7f1adull, 0xdf8ef6fb7570ff49ull, 0xc7ffbf7eb7ffee6aull, 0x99d7db7f5ef6fb7eull, 
	0xbbcf7ff65bef9748ull, 0xfdffffb43f1ffcb7ull, 0xfffaefffcec3f337ull, 0x5f3fe0c6fb7fb595ull, 
	0x5aa7ff9dffefe242ull, 0x57fdffb8c7ffe02dull, 0xf7f396f96dfda6abull, 0x5fefef3efedfefa4ull, 
	0x7becfab7edefe2a2ull, 0xb3ff6bccffff0cfaull, 0xffffffdabb9fabd6ull, 0xeffcbdadbd7f2243ull, 
	0x4efbffeddefa6c7cull, 0xfffaf7d3f766b43bull, 0xf7fffde6ddfe2c63ull, 0x7ffc77f3fdfeacb1ull, 
	0x1fbfabcee5ffbe06ull, 0xde5dd9ffdf6facadull, 0xffd7b6f85fb422fbull, 0xfd8ea2effbf788d5ull, 
	0x76ff71ed4b63c023ull, 0xfffa3bdff9fe9a85ull, 0x377eef8cbc2b8373ull, 0x7bfef6bb7f2dd3c0ull, 
	0xf9dbf777ffff83ddull, 0xffe76f7ffbbfee54ull, 0x9777f75fdddebbf2ull, 0xdbfcf5cfefbe7fa5ull, 
	0xdbffb5edff7f8dc4ull, 0xb6fa77f4cffee515ull, 0xfeff5e6fffdfebc8ull, 0xf6dfbf6dd1daf6b8ull, 
	0xeebf3dff357fa149ull, 0x3b50493dffffc755ull, 0xa7faf7ff762b5ca0ull, 0xdffffeffe9b59306ull, 
	0xf51ff3ffecef8f18ull, 0xfe776bfd3fff900aull, 0xcbb6e67d8bc7453bull, 0xff7bffdff9f7ef0cull, 
	0xcb6eefffbfffc1aaull, 0x75dfbbebf7fe9a8eull, 0xbbf7febbfefed9dbull, 0xdab7797f77dfa82full, 
	0xdfd7fddddfffebdcull, 0xfffae7e8f5dffcddull, 0xdfd5b9bfdfd193b0ull, 0x4f7efffffa5ba441ull, 
	0x7e6ffdadcff5d0fdull, 0xf9bcffc9cfce86efull, 0x77f4a6effd72be8full, 0xf9fffcbf97ffb432ull, 
	0xb7dfdf8faed6e801ull, 0xbfff9fff5fe9faabull, 0x35ffbf3ef7cfef2eull, 0x6ff796b7fbe5e26dull, 
	0xdb3f7ffdff6fc6deull, 0xff53bb3fbeb7e919ull, 0xbf2f3d6dfd468b33ull, 0xff25df73f71f3c11ull, 
	0xb35efeddefbe8b7bull, 0xbdfdbffdf775f146ull, 0xffefbdbccd6b815aull, 0x7e563f3fd33fd1b3ull, 
	0xdfff9ffcfefe1fedull, 0x4bffe6f8d663f37eull, 0xef8e7f0477bb820full, 0xf8aeede7facd0741ull, 
	0xffb4e516abc72913ull, 0xb79f3efb6dacdffeull, 0xb7ade7ffed7f8a8full, 0xabd7bfffc1d0bb3bull, 
	0xf1faaff7ffdfc685ull, 0xbfff6da5c7fb5f4cull, 0x474beff6cfffa413ull, 0xbcfb7eff6d38dd68ull, 
	0xbdfea686ef1cc207ull, 0xffaff7fac4b9b75dull, 0xb6efee7eefdbf7edull, 0xdfa67d97cfeec579ull, 
	0x7bdfdbefdf73c32aull, 0xfadbb7fb9f3ee058ull, 0x370bfef7cf7e8e03ull, 0x3e5ebbf0efbfb301ull, 
	0xf4f9bcd7e7dacd52ull, 0xf4dfb676edf3f29eull, 0xfa2dbffebff4498dull, 0xfeffffbf7deca3f1ull, 
	0xbacfb737ff5ffe96ull, 0xade563fbeafb809eull, 0xa16ffebbfffbbae7ull, 0xf7f67b21ffff60baull, 
	0xf5faf1fffff14fddull, 0x7dddbf7ebdbc492bull, 0xf486d9effeed03e9ull, 0x7dfbe7b1f56fe991ull, 
	0xafebf3fbfd5fb49full, 0xdf45ecf75d678ba8ull, 0x5b7d7b2ff76ed46bull, 0xbfeefeabbadb311aull, 
	0xfe3fbfffd97faeaeull, 0xfcfbfd5fff7fc5c4ull, 0xf9fffffd4b7d82f9ull, 0xffee79febe872b8eull, 
	0x9e93f7795bef8fd2ull, 0xfbfb8ebbd7f370a8ull, 0xbd7be5ffe7b79ce9ull, 0xddfffdbfcdfbdaebull, 
	0xbaffbbf5f9f5a145ull, 0xdfe6bf9ef573f8cbull, 0x397bffedff6fe98aull, 0xf7ffdffa59fb644full, 
	0xf6efaf735a369dc4ull, 0x9ddfa4dff774943eull, 0xfbebfad5affd84b6ull, 0xdf3ffddf463d408cull, 
	0x7ff2d77fbbbc86fdull, 0xfdcf2efb72f27957ull, 0xd8efbb16fa3fa777ull, 0x6fd77f83fac98bb1ull, 
	0xfffef2d8f0bad563ull, 0xbff6be35fbbdedf0ull, 0x66bef4beff7fdff3ull, 0xfedbffe6ff9ff7b1ull, 
	0xbf7dfeff5ffacf9cull, 0x3bafff6f77bf5765ull, 0x8edeafe1d9fb0bceull, 0xf3e3fd6ea6fed641ull, 
	0xaffebcbbf7e550bbull, 0x577ef76fedff28ddull, 0xbffbfcee7d7bc5d0ull, 0xbefb3fe7bfff1b34ull, 
	0x1bf7bfcf7befb725ull, 0x99ebebffdb77a8b4ull, 0xf77fec4bff2da778ull, 0xf5d5f74d693ed5feull, 
	0xdfd6f5d1ffff8378ull, 0xfe7a7fbfffea9ec1ull, 0x7ddf776f877c42f7ull, 0x7ef76632fff3e7c7ull, 
	0xdb7bf79fdf59da8cull, 0xde7dbefffacf0bacull, 0xbfbaffffffedddc0ull, 0xbffeefefffc78359ull, 
	0x7bcffbb6fe7fb946ull, 0xf7effeff5fa30ff4ull, 0x6dffbd3f81dfde3full, 0xb3edfe4efd7f8c81ull, 
	0xfff9aaf5ffdffc00ull, 0xfff3bbd7fd9f516dull, 0x6f69fdffff3786b7ull, 0xfbfdeffb74ffb0f6ull, 
	0xdf7a7b3aff9f64a3ull, 0xfdff7fcfaafffb2full, 0x57bb7e6cfbffba40ull, 0xdf9e76d3fffd4787ull, 
	0x7a3763fefdddcf54ull, 0xfddffb7f7f1ffb90ull, 0xfff7f5d387af8f24ull, 0xbbe5c7e67fefec18ull, 
	0xb5fbfe7d26b7da23ull, 0xf366f658edbfa8e5ull, 0x3fbd5effe95c41b0ull, 0x77c4d97fe7fe1881ull, 
	0xfbaff8ffe6e5d30dull, 0xdef7efffcabf4a27ull, 0xfdfefde759fda223ull, 0xfdfced7fbf5f9642ull, 
	0x5ff3fbbff4ab913cull, 0xfdd77feb7fdfbe3aull, 0x37c8fb47dffbb04aull, 0x7effede4fe36b204ull, 
	0xffffebf7da0e85a4ull, 0xdfbf673577ffc37eull, 0xfadfe73ef7ffac85ull, 0x3f8dab39ee9f76a7ull, 
	0xf1fe9cb77a5fa426ull, 0xfef7aff6ffbae12aull, 0xfdefdfdf7e6be75eull, 0xfb79bffff95fc4ecull, 
	0xfff3fabfffebbe1aull, 0x5bfdbeeffbdf95bcull, 0xb7cf7fa5ef6f8ba0ull, 0xaf57e3ed4eb9e574ull, 
	0xfcbbfcf8b6efbfc3ull, 0xf7cfffbdb7f1c7e9ull, 0xffc6eaf376e9834aull, 0xffbbf9dbe7fec651ull, 
	0x87ffbd7daa77cb40ull, 0xbc59f772fff733ddull, 0xf67fd73bc37f525cull, 0xe772d5fafdf3a580ull, 
	0x77afedbff3eff430ull, 0xd7eb7e5bb3f7b21bull, 0x77f6f9636f5fbb22ull, 0xffde1dffe9bf60f5ull, 
	0xfffeffde7dbf6fcbull, 0x7eff7f69f6fb9f67ull, 0xe3abfdfbb6c37a90ull, 0xfcccdd73f99d4cfcull, 
	0xfddffddedff48229ull, 0xb2ae9fe7e5d51da4ull, 0xfefff4e7d7ffa9feull, 0xbdbffebdb3fb8862ull, 
	0xf2747fdff9facb9cull, 0x77eefdf7fdbf3879ull, 0xb0f6bffb53bfde63ull, 0xed8df9f7f9bd1858ull, 
	0xf7fc6ef7ef368939ull, 0xb879bf975ffd88ecull, 0xf8f7feff4fcfcc40ull, 0x9dbfbbf77f9b5612ull, 
	0xdf3ffe7ffea219a1ull, 0xcd7fbd6607dbfd0aull, 0xfd6efbbf6edca7acull, 0x53ffe7fd7fb1a514ull, 
	0xdfcf5fcdf5ee9cfdull, 0xf6d7ef7fb6e6d1a0ull, 0xffbdefadefaf0305ull, 0xffffb3fd7cfbee89ull, 
	0xf9fdfefa6bbfff5dull, 0xf2777dd7d979f4b3ull, 0xf8e8afff9ad28704ull, 0x3f7a7feb4fd3aa36ull, 
	0x749f46b4fbe7e619ull, 0xf677fbeffdebda6dull, 0xb2db96eed9ef0b7bull, 0x7f9fffffc58fc4edull, 
	0x7dfdbffffd8ab00full, 0xee45a63f8bd3de7cull, 0xfdfdfb2bfe2ecd1full, 0xdc73fc97a9779464ull, 
	0xefbf767affdfa34aull, 0xf6fdfffff97db814ull, 0xfdade4656ffb9dafull, 0xfd6afef2f7e3ea9bull, 
	0x9fb37f7ffefbb3e9ull, 0xd4fbbfedd7ff52fbull, 0xebbbf7e7ef71c8b4ull, 0x7d7ef5bbecfddc7full, 
	0xbff17a7fd79be4fbull, 0xeffdfd7e6ebf9da6ull, 0xf7af3dfbbfd99f68ull, 0xfffeffebf77a1540ull, 
	0xbf9b3cdd71ee0432ull, 0x3ffef79bffbf9670ull, 0x5df36bfde6fcbe42ull, 0xd5be5f632edb0417ull, 
	0x7fdcfe7fd6bef5feull, 0xff6f7cfbaf13d915ull, 0xf73efe3affffae94ull, 0xdffe235fefefcb25ull, 
	0xffecbf8fd3fbb13full, 0xdb57b6f77f9f9eafull, 0xbbfae7b3b7fd40e6ull, 0xbd77fef2797348d8ull, 
	0xbffff2f38dbdb158ull, 0xbfecadf3dafd1ec4ull, 0x9ba7f7e3ffc9e6dcull, 0x777fe7fddff4906aull, 
	0xe6ef79953e7e87f3ull, 0xbf9df7fdbfff9b61ull, 0x5f7d7f69d74526b5ull, 0xb75dfb5fe8f3805eull, 
	0xfd9ed7b7ffef41e7ull, 0xffebfdbb5f7f5478ull, 0xbd5f77edfdf986b3ull, 0xffff9de7ea7f0c60ull, 
	0xff1fffd8ca5dbb6full, 0x5f8ff1fffeffcf07ull, 0xfbffdfefdb1dc43full, 0xc7f7ffdffae7a48full, 
	0x7fbff91fcfff8842ull, 0xee7be7dffb2eb1c1ull, 0xf9f7e5bc6fef6436ull, 0x36fe6fd7cd5f0bddull, 
	0xfefeefebef7d7337ull, 0xdaf7fff946baf71eull, 0x77f7bfe7ef770b88ull, 0xf9f7cbd7eeffc17full, 
	0xcdbabfeb5d76b75full, 0xffeda1bfeddf5c0full, 0xefacf75ff7ef0cd5ull, 0xdfffb5bbe9d5792dull, 
	0xffcff577f5eff19cull, 0xff36fffdf6dcca9eull, 0xff57faf7d4ce7473ull, 0xfffcf36ff7b75f80ull, 
	0xffb4f7afff7e3993ull, 0x2fdbff3ffbe967adull, 0x7bfefeffffdc03abull, 0xb9bf3a9adff2d912ull, 
	0xffffeffbbf7f829aull, 0x90bfbdda6ebb3454ull, 0xe2a5efc75dbfebfbull, 0xbff9fbfbf5efe22dull, 
	0xdffd9deaf7bb0691ull, 0x7fef5ab998efb11dull, 0xf7bd7df8bdf5f5fcull, 0x7388aff67cffc59cull, 
	0xfaf7ffedf9ffeaa9ull, 0x75fedf3b7bfbdfc1ull, 0xddebef3f8fb48c73ull, 0x5a5fa1ff3e5ef026ull, 
	0xf766ef5beb6eefb3ull, 0x7f77bb5ffff8f341ull, 0xdedf7cbd7ebfcf96ull, 0xf7ffe6bffefcdaddull, 
	0xf3f6bffbfdbd6a8dull, 0x4bff91fbc7ffdab8ull, 0xfbfaefde757ed02full, 0xbab17da7efeea099ull, 
	0x76bf7f3cffff4154ull, 0x5feef8347f773297ull, 0x584fa76ffbff8679ull, 0x5f57fdee2fceb783ull, 
	0x7f1fe5c6ff2f42b4ull, 0xfc76beb7665fef06ull, 0x3fbffdbfc3fce836ull, 0x6bc1fd7adbbfad32ull, 
	0xfdb9ef396e7f63adull, 0x7f77fcff5fa3fd95ull, 0xfffeff765dedec3cull, 0xaff6fd7bbfbf8cc6ull, 
	0x2cfffcbf7beca26aull, 0xbffde9bffdfdc9fcull, 0xfbbfb7fdedcff4f4ull, 0xdbea7b2f6dffab57ull, 
	0x7d7bf95ebdf8030cull, 0x79b5f77e3c7f2c60ull, 0xff9737797dbfb940ull, 0xefb6d6cfd33ab6a3ull, 
	0x0fff7a6fd6fd1a32ull, 0xdbd359bfff84aa00ull, 0x76feb33b5bda637bull, 0x7bb7fdcdfefbffcaull, 
	0x3d7ffd7aab656c5dull, 0xebeff76effa93d08ull, 0xda9f3f7ffcbd6ad3ull, 0xb2ffff8f7f7030e2ull, 
	0xff9ef7effffdf372ull, 0xfb6fbbddfe309c3full, 0x3ffa4bff7f73a0c7ull, 0xf7de7bb9dfffd1f9ull, 
	0xfff7ffd72ccafddbull, 0xdebdf97b4f955b68ull, 0xfa9febcdbeff719eull, 0xfafb6ffef76f2665ull, 
	0xbdfbeff7ffd48a9cull, 0xdf6bb7b34d8784e2ull, 0xf6ff6ffcfdf228a7ull, 0xf45df82fffad9e48ull, 
	0xf26d77777e3dec2bull, 0x7b737e59dcef9f28ull, 0xb5e46990799ff05full, 0xdfeb757effbbf274ull, 
	0xffbb75fe7cfdb810ull, 0xd6ebff5f876df307ull, 0x7bfe7775cfbfb219ull, 0xcf7fffa6ffed8a9aull, 
	0xb8cfebbebfad7c3aull, 0x2bb7fbfffffabc32ull, 0xfb6fffa727ca8b87ull, 0xff7fe92affff9df9ull, 
	0xf77fc72bfea595eaull, 0xbff3bb5f53fdcb02ull, 0x7bdeebfbdbef340dull, 0xffddded7cdfde927ull, 
	0xfefa3fd9cbf7b175ull, 0xff55fe87d7fde42dull, 0xefffeeffff4d12d5ull, 0xff33bf2c7b073544ull, 
	0xb735fd7e3bf52302ull, 0xdd61f7fbed7f17cbull, 0xdcfc3febfb6db87bull, 0xc9cbfde7ef77e848ull, 
	0x9f6d5ccfeadf005bull, 0xfbf7fdeffed78c8eull, 0x7fef3b7bfdfbe3e0ull, 0x6dfbeffdaf9e998full, 
	0x7ebfbdedf8eb4e43ull, 0x5f0ddce5fd5ffacaull, 0xf7baf8fafbeec71full, 0x7f7c2db7e3f72202ull, 
	0xa97beedbfed0a613ull, 0xfdcfdfafdbe9a554ull, 0xb679abfafafffed0ull, 0xab7fff3b7f3df857ull, 
	0xf95bfbdfcfffa915ull, 0x5dfef4cf767998a7ull, 0xf51778d5ffffa557ull, 0xacbb6d66f73fc2bcull, 
	0xfedb5aa7f7779306ull, 0xeffef0ffdffff2d2ull, 0xf7f7367ffff7c0b9ull, 0xffaf9d4dbe7b6644ull, 
	0xbfd3b7fc6ff93547ull, 0xfe73edafdfbeae6bull, 0xe4bffef37747c4c5ull, 0x6f8f7fddff3fae78ull, 
	0xbebff7fddcfdcb6eull, 0xffee7bdee7d7631dull, 0x7f3cff7bf57da2a2ull, 0xb7eafe76aff6a87aull, 
	0x7dfbefe6f9ef52f7ull, 0xfefd1ff3fe3e2254ull, 0x85d5e7feff7f26b5ull, 0xd5bffccffdf77f4cull, 
	0xc329f5fdefdfa75dull, 0xbfff77ea59ee848aull, 0xbbf7ffef6ffeae1dull, 0xefef36f7febe97dbull, 
	0x74cde1dfbefa8682ull, 0xafe8fba393dc9a09ull, 0x362f47f47372eaf9ull, 0xdf7cffff7ffd563dull, 
	0x97e8b2ff76bace90ull, 0xff3476e5afef9f4eull, 0xfde7af95ffd9e0ccull, 0xfbd3a7edbfa7b077ull, 
	0xefff27ffd7bed4feull, 0x4fbbfbbf77ffcd22ull, 0xec5fddaf7df3a0c5ull, 0xddebfefee8ecfdc9ull, 
	0x5dc6eeebf796400bull, 0xbcc9f7fbce5b9e91ull, 0xff2f7bf6ffbb4a39ull, 0x5fbfffe3dfc6fe87ull, 
	0xff7b37f3c7ffa528ull, 0xf5ec7ebf3fef9978ull, 0xfdfffdf7f7ffed61ull, 0xf775fd0bfb3ec984ull, 
	0x3fffdb2bf9b5af21ull, 0x31fe86db7bbed1d0ull, 0xfdb3bb795fff66c5ull, 0xd675fefdff879e94ull, 
	0xbbbfff6fff37a2dcull, 0xfe2dfbf5fdbfcd09ull, 0xfeb6fd37febfd99bull, 0xb61df7dfffffaceaull, 
	0x1c78eef39fe935deull, 0xdfefeffbfd7dda92ull, 0x7f7af6feffefc5f6ull, 0xfce9f77fee7df82bull, 
	0x7ef7aeffff6e927eull, 0xfffee7ff6fff9dc7ull, 0x7d1bffbedcff2782ull, 0x5fe6e3ffdfffc37bull, 
	0xfefcb37b62ef6352ull, 0xfffbebf7d7effb7aull, 0xfdfdfcffd9f3d2f3ull, 0xfebfadf7df7ed131ull, 
	0x5dd0b27ff73f85a6ull, 0xa4bffa37d7fe15f1ull, 0xfc51bffff4dcd963ull, 0x5fffbbfd3f8ff4ebull, 
	0xdfddfcc6adfbfdf5ull, 0xfbee3fe2d27ae974ull, 0x9defaf7ff67ba714ull, 0xf4f7f7fdaffdfe9full, 
	0xfebfbd1cdeef7096ull, 0xfbed54ffede872c4ull, 0x2cf9eb76efbff076ull, 0xb9fbfad5d7fd8710ull, 
	0xbf7bbfffdffed055ull, 0xdecfefe5f12f82c2ull, 0x33f673cffa6fd6c0ull, 0xffbf9cdcfffaab3eull, 
	0xbffe7f2eff7fcb56ull, 0xf7fabfdf3fdfe3fdull, 0xfb73fcff7ffd99c0ull, 0x527dde7776fee7edull, 
	0xfb2ed3f6fd1f4956ull, 0xfefbfcfdbe5b2073ull, 0xf58fb77f7fffcfb1ull, 0xf77bf7cff77f665dull, 
	0xdfa7deda6fdfda3bull, 0x7fdbfdddddfefcd3ull, 0xe7eefdefdbff3865ull, 0x1d4fffd76ff576f0ull, 
	0xfdfdfdbadfff3f36ull, 0x3efcdbb77dcba648ull, 0x9cfebffeffd1ca8cull, 0x557ebbe7fffec570ull, 
	0xfbb6ff77fb3f8701ull, 0x7bfbacefffdb82bfull, 0xebededeffd2f7698ull, 0x7afffe753fe7e620ull, 
	0xe3beede6efbd2815ull, 0xa5fbea2efd72a03cull, 0xff7efade4fefc99dull, 0xcfdee7febfff8dd7ull, 
	0xf7cfbdaeefdc08b8ull, 0x599cfa9eefcf03dfull, 0xff7befffdb7ca54aull, 0x7eb5ebc3bf5fcc6eull, 
	0xfffdff37df2a2a3dull, 0xefefedfbdf3f403cull, 0xabc7eefff7faf57bull, 0x3dff7fff2fff8bc3ull, 
	0xfbefe8ff7ff34e74ull, 0xabffbfdffdfdeb02ull, 0x9f93f777fdf4cf50ull, 0xf79dffbbdcef8604ull, 
	0x9ffb7ffbd7ffa3dcull, 0x5ffef7e3f56a8bc6ull, 0xfdcfea17ccf74b26ull, 0xff56ff9febc2f35aull, 
	0x7e7ef7f7bd1d8f05ull, 0xdfd6aa26fefeedc6ull, 0x7dffeefdeb2e3809ull, 0x5afff6dfbd9db7b9ull, 
	0xf5687bd6ffbb8229ull, 0x3fdfffdd5fcfc5b8ull, 0xf7f8ff97aeff8bf1ull, 0x7ff7effafee7d681ull, 
	0x3dfafe7cdfd7b1a7ull, 0xfef7ef6fdb7c5ab6ull, 0xcffd7eaf3f7c8fafull, 0x81bffc0eea57e817ull, 
	0xf4dd7bffecb6c686ull, 0xd3fdf7f4fdf78100ull, 0x9afbef5ce6ecd684ull, 0xffbf76bc558e97f5ull, 
	0xdfbf7efff4b913bfull, 0x77ae636f5b55cc2cull, 0xebffaffeffff8f69ull, 0xee3db8fe743e8226ull, 
	0x6beb5ffbdfff9d8eull, 0x8beeecfdbbfa8227ull, 0xbfb4db8afafb840eull, 0xf7dfeff5b7f7b238ull, 
	0xfebdba7fb72dd85aull, 0xfbc07797fbbdb0dbull, 0x7bcb3bfeffed8a49ull, 0xddfbfdf1ef3d378dull, 
	0xf777f98db7ffc830ull, 0xdefbfdf7777fc2f0ull, 0xf9adfdebf8dfbc0cull, 0xf5fd9ff7fbfd9f5dull, 
	0xeffe2efd5bf9f539ull, 0xde7df87ffee7fc22ull, 0x7fff3ff6fdff154bull, 0xfffff9b67e38a169ull, 
	0x7ffb7399cfbda5e5ull, 0xdef973dbdffc29aeull, 0x6f7ff746cfe2986eull, 0xbdfaff75fd1da7ccull, 
	0x7f37f7ed4fad6442ull, 0xf96f7579dfeb7dbaull, 0xbfffffecf333c90bull, 0xfdff7dbafb7ffe13ull, 
	0xffb3ff737afa5904ull, 0x3efefb4fefefba12ull, 0xdffdb17f5fdefcdbull, 0xeeffc737ef3f9e38ull, 
	0xfdbf687d1e3ea298ull, 0xf6fbfa3d6f36be5full, 0xdbfd3dffc62fddfcull, 0xbfb7e77dccefb0aeull, 
	0xffebb95adbecc499ull, 0x773fff54f7c7f9b0ull, 0xfd1e7ffbd77bc7cfull, 0xd7f7dffe57ef2144ull, 
	0xd8bff17fffdbcde7ull, 0xefdffff7fbaa4708ull, 0x5ff8f9efdffe511aull, 0x7cebf7bff7fbf9c4ull, 
	0xbbfff9ffd7f3f80full, 0xbbb5bfeb7ef29a93ull, 0xbfffbfe79dd9b80aull, 0xea7f77ffaeffe98bull, 
	0xbeddd7efdd7e8fa1ull, 0xccfdf7fffc9b877aull, 0x3cdfed9bfffb80e9ull, 0xff7be7bdb0e3fd6aull, 
	0x7dcb6febd9f5afccull, 0xa77f7db55fffa3bbull, 0x7ff7ec7bcdf5a090ull, 0xf85ffeda7fdfb947ull, 
	0x7ed29fffe0f7d071ull, 0x9c9ffbf67dffe74bull, 0xbff909fcdbecc0c6ull, 0xff6de9b2efc9b4b9ull, 
	0x2e3eafde65ffa010ull, 0xffffeef8fd2e3e70ull, 0xbedfffd0fe969b26ull, 0xd1fe3fa5feffe599ull, 
	0x15a77fff6c3c8216ull, 0xf5dfbab1fefeec95ull, 0xfdcdac6f17770cd1ull, 0xf7b8bbfedb7cf11eull, 
	0x5cbfffd7fff324fbull, 0x9fb7ff3fc3df9f05ull, 0xbf9b39fcfffb1e98ull, 0xcbfd74be85ffb2c0ull, 
	0xedbebffedf7ff818ull, 0xbb3bfb2dcdfbaeedull, 0x7cb4b7bfffe5a4c9ull, 0xdfffdbffd31dad7eull, 
	0x5cbdea7fdef37e64ull, 0xfbbeefbbbe77825dull, 0x43c7dfffcef7ceeaull, 0xff7ff57fffe1cbdaull, 
	0x2fdc64e6afbf97eaull, 0xfb57fdabeb38f35eull, 0x3fbfc9b5fddf9c4full, 0x987effeee45bc426ull, 
	0x6dd39befeaff8cd0ull, 0xdecb7f35bf3fa12bull, 0xffd7bfb58cff72a5ull, 0xfafb67df7fcd9075ull, 
	0x7d4fffdd7defe381ull, 0xfdfef5cff73f6c38ull, 0xdbfeedfdfd985e8cull, 0xaff7ff4d7d7fe2e6ull, 
	0xfeebbff3ffbfe5fdull, 0xfb8dfc8fe243971eull, 0xfecdfbbdffedc691ull, 0xf9efee6d745fc7e9ull, 
	0xf9b9fcfbdfd549f8ull, 0xe4b71fdfbff8b08cull, 0xf3fdee97efd7b7a0ull, 0xbb9fd77d7fa69aafull, 
	0x5df4575afdff9fccull, 0xde7f1dfcbfe5bb53ull, 0xfff7fbd55ef8cb5full, 0xb6efe7fbf7dfd832ull, 
	0xff9dfb5ef7884a9bull, 0xffabbfe6dfede0d8ull, 0xbef3fffafef58f67ull, 0xfff86f9deff7af42ull, 
	0x7a229bfff3bde9c0ull, 0xf7f77ff7dca7f8cbull, 0xfffdf7f22febe6b6ull, 0x5ffe75b7bdfbd0c7ull, 
	0xfdddf5bfff5de6a0ull, 0x7e4a3ebcffff863aull, 0xf32ffdd7cd5d8a8full, 0xb5bdfd3b5bdfd897ull, 
	0xf7bfa7ff88ddf124ull, 0x75f7b15fc7dbce35ull, 0xfe727feffff84ed5ull, 0xff97b7cf93deb58bull, 
	0xffdffc9e6f77a7daull, 0xff7f73dd7bfddd46ull, 0xddadadfff77ba936ull, 0x3bfdbb7ffd53ca5dull, 
	0xef9fa8f8fdebbdfdull, 0xff74fbe1fd24f7f3ull, 0xf7bffd56fff1bb92ull, 0xbdfeb277ef7c487dull, 
	0x5bbdffe3bb0e2ab9ull, 0xf417f7e5fbb6fb46ull, 0xbb77eb3f2f7db848ull, 0x5f8aff797fffc882ull
};

const uint64_t Hash::SideKey = 0xdfbfd779fffbf846ull;

const uint64_t Hash::CastleKeys[16] = {
	0x7fc37a716bffa2abull, 0xffcff67bff731cecull, 0x7fdf769f7fdafb63ull, 0xfff67dffe9f54cf6ull, 
	0xbb0f7fa3d77fcaa7ull, 0xfdbfff7eee779bcfull, 0xff6fbb1feff5cb1aull, 0xffeef6dbdead8744ull, 
	0x1fffff3bfdfb057cull, 0xffb5f53ede7f94e0ull, 0xfdcf4af7ff5ff040ull, 0xb5bff7f38d0df70eull, 
	0x764fe97dffffee16ull, 0xfaffbae7afdf813cull, 0xfdfc7d613ce3f056ull, 0xfbfe9f7f7afbc991ull
};