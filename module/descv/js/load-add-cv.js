var cv = jQuery.noConflict();
cv(document).ready(function(){ 
    cv("#create-cv").on('click', function(){
        var infos = {
                tieude: get_thongtin('.cv-title'),
                anhdaidien: get_thongtin('.cv-title'),
                hoten: get_thongtin('.cv-item-hoten'),
                vitrimongmuon: get_thongtin('.cv-item-vitrimongmuon'),
                sdt: get_thongtin('.cv-item-sdt'),
                email: get_thongtin('.cv-item-email'),
                ngaysinh: get_thongtin('.cv-item-ngaysinh'),
                gioitinh: get_thongtin('.cv-item-gioitinh'),
                diachi: get_thongtin('.cv-item-diachi'),
                website: get_thongtin('.cv-item-website'),
                sapxep: get_thongtin('.cv-item-title-contact'),
                muctieu: get_thongtin('.cv-item-muctieu')
        };
        var kynangs = get_kynang('.item-kynang');
        var sothichs = get_sothich('.item-sothich');
        var nguoithamchieus = get_thongtin('.cv-item-thamchieu', 'html');
        var hocvans = get_hocvan('.item-hocvan');
        var kinhnghiems = get_kinhnghiem('.item-kinhnghiem');
        var hoatdongs = get_hoatdong('.item-hoatdong');
        var chungchis = get_chungchi('.item-chungchi');
        var giaithuongs = get_giaithuong('.item-giaithuong');
        var thongtinthems = get_thongtin('.cv-item-them', 'html');
        cv.ajax({
            url: "/cv/add",
            type: "post",
            data: {
                info : infos,
                kynang: kynangs,
                sothich: sothichs,
                nguoithamchieu: nguoithamchieus,
                hocvan: hocvans,
                kinhnghiem: kinhnghiems,
                hoatdong: hoatdongs,
                chungchi: chungchis,
                giaithuong: giaithuongs,
                them: thongtinthems,
                  },
            success: function(result){
                cv("#create-cv").remove();
           cv("#edit-cv").html('<div class="messages status">Đã tạo CV:' + get_thongtin('.cv-title') + '. <a href="/cv" class="">Chỉnh sửa</a></div>'); 
               // cv("#edit-cv").html(result);
            },
        });
    })
    // GET VALUE FORM // THÔNG TIN
    function get_thongtin(cl, type = 'text') {
        if(type == 'html') {
            return cv(cl).html();
        }
        return cv(cl).text(); 
    }

    // GET VALUE FORM // KỸ NĂNG
    function get_kynang(cl) {
        var loai = new Array();
        var mucdo = new Array();
        cv(cl).each(function(){
            loai.push(cv(this).find('.cv-item-kynang').text());
            mucdo.push(cv(this).find('.kynang-value').attr('vl').replace('v',''));
        });
        var result = {
            loai: loai,
            mucdo: mucdo,
        };
        return result;
    }
    // GET VALUE FORM // SỞ THÍCH
    function get_sothich(cl) {
        var sothich = new Array();
        cv(cl).each(function(){
            sothich.push(cv(this).find('.cv-item-sothich').text());
        });
        return sothich;
    }
    // GET VALUE FORM // HỌC VẤN
    function get_hocvan(cl) {
        var batdau = new Array(), ketthuc = new Array() , tentruong = new Array(), nganh = new Array(), chitiet = new Array();
        cv(cl).each(function(){
            batdau.push(cv(this).find('.cv-item-hocvan-batdau').text());
            ketthuc.push(cv(this).find('.cv-item-hocvan-ketthuc').text());
            tentruong.push(cv(this).find('.cv-item-hocvan-tentruong').text());
            nganh.push(cv(this).find('.cv-item-hocvan-nganh').text());
            chitiet.push(cv(this).find('.cv-item-hocvan-chitiet').html());
        });
        var result = {
            batdau: batdau,
            ketthuc: ketthuc,
            tentruong: tentruong,
            nganh: nganh,
            chitiet: chitiet,
        };
        return result;
    }
    // GET VALUE FORM // KINH NGHIỆM
    function get_kinhnghiem(cl) {
        var batdau = new Array(), ketthuc = new Array() , tencty = new Array(), vitri = new Array(), chitiet = new Array();
        cv(cl).each(function(){
            batdau.push(cv(this).find('.cv-item-kinhnghiem-batdau').text());
            ketthuc.push(cv(this).find('.cv-item-kinhnghiem-ketthuc').text());
            tencty.push(cv(this).find('.cv-item-kinhnghiem-tencongty').text());
            vitri.push(cv(this).find('.cv-item-kinhnghiem-vitri').text());
            chitiet.push(cv(this).find('.cv-item-kinhnghiem-chitiet').html());
        });
        var result = {
            batdau: batdau,
            ketthuc: ketthuc,
            tencty: tencty,
            vitri: vitri,
            chitiet: chitiet,
        };
        return result;
    }
    // GET VALUE FORM // HOẠT ĐỘNG
    function get_hoatdong(cl) {
        var batdau = new Array(), ketthuc = new Array() , tentochuc = new Array(), vitri = new Array(), chitiet = new Array();
        cv(cl).each(function(){
            batdau.push(cv(this).find('.cv-item-hoatdong-batdau').text());
            ketthuc.push(cv(this).find('.cv-item-hoatdong-ketthuc').text());
            tentochuc.push(cv(this).find('.cv-item-hoatdong-tentochuc').text());
            vitri.push(cv(this).find('.cv-item-hoatdong-vitri').text());
            chitiet.push(cv(this).find('.cv-item-hoatdong-chitiet').html());
        });
        var result = {
            batdau: batdau,
            ketthuc: ketthuc,
            tentochuc: tentochuc,
            vitri: vitri,
            chitiet: chitiet,
        };
        return result;
    }
    // GET VALUE FORM // CHỨNG CHỈ
    function get_chungchi(cl) {
        var ten = new Array(), thoigian = new Array();
        cv(cl).each(function(){
            ten.push(cv(this).find('.cv-item-chungchi-ten').text());
            thoigian.push(cv(this).find('.cv-item-chungchi-thoigian').text());
        });
        var result = {
            ten: ten,
            thoigian: thoigian,
        };
        return result;
    }
    // GET VALUE FORM // GIẢI THƯỞNG
    function get_giaithuong(cl) {
        var ten = new Array(), thoigian = new Array();
        cv(cl).each(function(){
            ten.push(cv(this).find('.cv-item-giaithuong-ten').text());
            thoigian.push(cv(this).find('.cv-item-giaithuong-thoigian').text());
        });
        var result = {
            ten: ten,
            thoigian: thoigian,
        };
        return result;
    }
/// HOVER SHOW BUTTON ADD, REMOVE ITEM
    cv('body').on('mouseenter', '.n-item', function(){
        if(cv(this).parent().children('.n-item').length == 1) {
        cv(this).append('<div class="item-edittool"><span class="up-tool tool">▲</span><span class="down-tool tool">▼</span><span class="add-tool tool">Thêm</span></div>');
        } else {
        cv(this).append('<div class="item-edittool"><span class="up-tool tool">▲</span><span class="down-tool tool">▼</span><span class="add-tool tool">Thêm</span><span class="del-tool tool">Xóa</span></div>');
        }
    });
    cv('body').on('mouseleave', '.n-item', function(){
       cv(this).find('.item-edittool').remove();
    });
/// BTN SWAP, ADD, REMOVE
    cv('body').on('click' , '.add-tool', function(){
        var a = cv(this).parents('.n-item').clone();
        a.find('.item-edittool').remove();
        a.appendTo(cv(this).parents('.cv-block-body'));
    });
    cv('body').on('click', '.del-tool', function(){
        cv(this).parents('.n-item').remove();
    })
/// NOT EMPTY
    
    
    
    
    
    
    
});