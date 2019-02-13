C $Id: mclzldist.f,v 11.7 2014/05/31 21:23:21 lyonsd Exp $
C $Log: mclzldist.f,v $
C Revision 11.7  2014/05/31 21:23:21  lyonsd
C Fixed a problem with argv
C
C Revision 11.6  2014/05/29 03:40:46  lyonsd
C Added command line options for min and max n
C Added command line option for input and output files
C Reversed order of columns for consistency with other *.cs files
C
C Revision 11.5  2014/03/07 19:26:38  lyonsd
C Fe^26+ + H
C
C Revision 10.5  2014/03/06 19:47:18  lyonsd
C O^6+ + H
C
C Revision 10.0  2014/03/06 19:31:32  lyonsd
C O^6+ + He
C
C Revision 9.5  2014/03/06 18:47:51  lyonsd
C Ne^8+ + H
C
C Revision 9.1  2014/03/06 18:46:55  lyonsd
C Ne^8+ + H
C
C Revision 9.0  2014/03/06 18:22:04  lyonsd
C Ne^8+ + He
C
C Revision 8.7  2014/03/06 18:06:32  lyonsd
C Mg^9+ + H (triplets)
C
C Revision 8.4  2014/03/06 17:41:48  lyonsd
C Ne^9+ + H (singlets)
C
C Revision 8.0  2014/03/06 00:13:40  lyonsd
C Ne^9+ + He
C
C Revision 7.5  2014/03/05 23:42:45  lyonsd
C Ne^10+ + H
C
C Revision 7.0  2014/03/05 22:51:16  lyonsd
C Ne^10+ + He
C
C Revision 6.5  2014/03/05 21:29:43  lyonsd
C Mg^8+ + H
C
C Revision 6.0  2014/03/05 19:54:51  lyonsd
C Mg^8+ + He
C
C Revision 5.7  2014/03/05 19:24:46  lyonsd
C Mg^9+ + H (triplets)
C
C Revision 3.5  2014/03/01 09:14:41  lyonsdav
C Mg^12+ + H
C
C Revision 3.1  2014/03/01 05:26:50  lyonsdav
C Mg^12+ + He
C
C Revision 2.1  2014/03/01 04:10:21  lyonsdav
C Mg^12+ + He
C
C Revision 1.30  2014/03/01 04:02:51  lyonsdav
C Mg^12+ + He
C
C Revision 1.12  2014/02/28 06:20:06  lyonsdav
C Fe^26+ + H
C
C Revision 1.11  2014/02/25 22:16:17  lyonsdav
C Ne^10+ + He
C
C Revision 1.10  2014/02/25 21:31:54  lyonsdav
C Ne^10+ + H
C
C Revision 1.9  2014/02/24 15:58:16  lyonsdav
C Mg^11+ + H (singlets)
C
C Revision 1.8  2014/02/23 07:40:24  lyonsdav
C Fe^26+ + H
C
C Revision 1.7  2014/02/23 06:08:40  lyonsdav
C Mg^10+ + H
C
C Revision 1.6  2014/02/22 17:43:49  lyonsdav
C Mg^10+ + He
C
C Revision 1.5  2014/02/22 02:24:51  lyonsdav
C Mg^11+ + H (triplets)
C
C Revision 1.4  2014/02/22 01:33:10  lyonsdav
C Mg^11+ + H (singlets)
C
C Revision 1.3  2014/02/21 21:30:34  lyonsdav
C Mg^12+ + He
C
C Revision 1.2  2014/02/21 20:59:17  lyonsdav
C Mg^12+ + H
C
C Revision 1.1  2014/02/21 01:21:36  lyonsdav
C Initial revision
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C mclzldist.f
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C Code to create nl-resolved cross sections
C from n-resolved cross sections assuming
C one of two distributions: i) statistical
C and ii) low-energy as given in Krasnopolsky,
C Greenwood, and Stancil 2004, Space Sci.
C Reviews, 113, 271; eqs. (4.6) and (4.7)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C May 29, 2013 PCS
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C input - fort.15
C output - units 8 and 9
C edit paramters and file names
C also edit in output file the quantum states
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      program mclzldist
         implicit none
         integer :: i
         integer :: n
         integer :: j
         integer :: nlow
         integer :: nhigh
         integer,parameter :: nmax=50
         integer :: l
         integer :: narg
         character(20) :: argv(4)
C EDIT
C These three parameters may need to be changed for each system
         double precision :: energy
         double precision :: cross(nmax)
         double precision :: crosst
         double precision :: crossnl(nmax,nmax)
         double precision :: crossnll(nmax,nmax)
         double precision :: faq
         character :: tab*1
         character(8)  :: date
         character(10) :: time
         character(5)  :: zone
         integer,dimension(8) :: values
         character(len=24) :: statfile
         character(len=24) :: lowefile
         character(len=20) :: basefilename
         character(len=25) :: infilename
         character(len=*),parameter :: lowesuf="_lowe.cs"
         character(len=*),parameter :: statsuf="_stat.cs"
         
         tab=char(9)

         narg=iargc()
         if(narg .lt. 4) then
            write(*,*)
     . "Usage: nlow nhigh mclzldist inputfile outputfilebasename"
            stop
         end if
         do i=1,narg
             call getarg(i,argv(i))
             if(i .eq. 1) then
                 read(argv(i),'(I3)') nlow
             else if(i .eq. 2) then
                 read(argv(i),'(I3)') nhigh
             else if(i .eq. 3) then
                 infilename=argv(i)
             else if(i .eq. 4) then
                 basefilename=argv(i)
             end if
         end do
         lowefile=trim(basefilename)//lowesuf
         statfile=trim(basefilename)//statsuf
C EDIT
C Change the filenames based on the system with which is being studied.
         open(unit=8,file=statfile)
         open(unit=9,file=lowefile)
         open(unit=15,file=infilename)

         call date_and_time(date,time,zone,values)
C read header

C         read(15,*) junk
C         read(15,*) junk
C         read(15,*) junk

C write header to the cs files

         write(8,999) date,values(1),values(2),values(3)
         write(8,997)
         write(8,996)
         write(8,995)
         write(8,994)
         write(8,993)
         write(8,992)
         write(8,991) ((n,l, l=n-1,0,-1), n=nhigh,nlow,-1)
         write(9,999) date,values(1),values(2),values(3)
         write(9,997)
         write(9,996)
         write(9,990)
         write(9,994)
         write(9,993)
         write(9,992)
         write(9,991) ((n,l, l=n-1,0,-1), n=nhigh,nlow,-1)

         do i=1,2000
            read(15,*,end=91) energy, (cross(j), j=nhigh,nlow,-1),crosst
            do n=nhigh,nlow,-1
               do l=n-1,0,-1

c statistical l-distribution
c eq. (4.6) of Krasnopolsky et al.
                  crossnl(n,l+1)=cross(n)
     .                          *(2.0d0*dfloat(l)+1.0d0)
     .                          /dfloat(n)**2 
c eq. (4.7) of Krasnopolsky et al.
                  crossnll(n,l+1)=cross(n)
     .                           *(2.0d0*dfloat(l)+1.0d0)
     .                           *(faq(n-1))**2
     .                           /(faq(n+l)*faq(n-l-1))
               end do
            end do
            write(8,998) energy,tab,((crossnl(n,l+1),l=n-1,0,-1), 
     .                   n=nhigh,nlow,-1),crosst
            write(9,998) energy,tab,((crossnll(n,l+1),l=n-1,0,-1), 
     .                   n=nhigh,nlow,-1),crosst
         end do
         close(8)
         close(9)
         close(15)

998      format(1pe10.3e3,a1,999(1pe11.4e3,2x))
999      format('# Magic-number='A8', 'I4'/'I0.2'/'I0.2)
997      format('# SEC cross sections, nl state-selective')
996      format('# Ion^Z+ + Atom -> Ion^(Z-1)+ + Atom+')
995      format('# Method=MCLZ, statistical nl-distribution')
990      format('# Method=MCLZ, low-energy nl-distribution')
994      format('# First Last et al., 2013, to be submitted')
993      format('# ------------------------------------------------')
992      format('# Energy    Cross sections (10^-16 cm^2) n,l')
991      format('# (eV/u)',999(i10','i2))
91       close(15)
      end program

C***************** Compute the factorial of v ******************

      double precision function faq(v)
C     *** calculates the factorial of z
         implicit real*8 (a-h,o-z)
         implicit integer*8(I)
         parameter(a0=1.0d0)
         parameter(a1=-0.5748646d0)
         parameter(a2=0.9512363)
         parameter(a3=-0.6998588)
         parameter(a4=0.4245549)
         parameter(a5=0.1010678)
         integer v
         integer z
C
         z=v
         if(Z.ge.0.) then
            r=z-int(z)
            p=((((a5*r+a4)*r+a3)*r+a2)*r+a1)*r+a0
            r=1.d0
            s=z
            do while(s.gt.z-int(z))
               r=r*s
               s=s-1
            end do
            q=r*p
         else
            if(z.gt.-1.) then
               r=(z+1)-int(z+1)
               p=((((a5*r+a4)*r+a3)*r+a2)*r+a1)*r+a0
               q=p/(z+1)
            else
               q=9.99999d9
            end if
         end if
         faq=q
         return
      end function
 
